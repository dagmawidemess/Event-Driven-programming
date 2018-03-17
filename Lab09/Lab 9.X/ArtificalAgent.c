/**
 * Authors: Michael Justin, Dagmawi Demess
 * Emails: mjustin@ucsc.edu, ddemess@ucsc.edu
 * Date: 03/12/2018
 */

// Agent Header File
#include "Agent.h"

// Standard libraries


// User Defined Libraries
#include "Field.h"
#include "Oled.h"
#include "Protocol.h"
#include "OledDriver.h"
#include "FieldOled.h"

// **** Set any macros or preprocessor directives here ****
#define OLED_PRINT_ERROR(x) do {OledClear(OLED_COLOR_BLACK); \
                            OledDrawString(x); \
                            OledUpdate(); \
                            FATAL_ERROR();} while (0)

/*****Declare Function Prototypes******/
uint8_t AddRanomBoat(Field * f, BoatType boatType);
void GenerateGuessData(GuessData *data);


/*****Declare Module and Global Level Variables******/
static Field enemyField;
static Field localField;

void AgentInit(void)
{
    OledInit();

    FieldInit(&localField, FIELD_POSITION_EMPTY);
    FieldInit(&enemyField, FIELD_POSITION_UNKNOWN);

    while (!AddRanomBoat(&localField, FIELD_BOAT_SMALL));
    while (!AddRanomBoat(&localField, FIELD_BOAT_MEDIUM));
    while (!AddRanomBoat(&localField, FIELD_BOAT_LARGE));
    while (!AddRanomBoat(&localField, FIELD_BOAT_HUGE));

    FieldOledDrawScreen(&localField, &enemyField, FIELD_OLED_TURN_NONE);

    //    int i = 0;
    //    int j = 0;
    //
    //    for (i = 0; i < FIELD_ROWS; i++) {
    //        for (j = 0; j < FIELD_COLS; j++) {
    //            printf(" %d ", localField.field[i][j]);
    //        }
    //        printf("\n");
    //    }
    //    printf("\n");

}

uint8_t AddRanomBoat(Field *f, BoatType boatType)
{

    int row = rand() % FIELD_ROWS;
    int col = rand() % FIELD_COLS;

    BoatDirection dir;
    int randDir = rand() % 4;
    switch (randDir) {
    case 0:
        dir = FIELD_BOAT_DIRECTION_NORTH;
        break;
    case 1:
        dir = FIELD_BOAT_DIRECTION_SOUTH;
        break;
    case 2:
        dir = FIELD_BOAT_DIRECTION_EAST;
        break;
    default:
        dir = FIELD_BOAT_DIRECTION_WEST;
        break;

    }
    return FieldAddBoat(f, row, col, dir, boatType);
}

/**
 * The Run() function for an Agent takes in a single character. It then waits until enough
 * data is read that it can decode it as a full sentence via the Protocol interface. This data
 * is processed with any output returned via 'outBuffer', which is guaranteed to be 255
 * characters in length to allow for any valid NMEA0183 messages. The return value should be
 * the number of characters stored into 'outBuffer': so a 0 is both a perfectly valid output and
 * means a successful run.
 * @param in The next character in the incoming message stream.
 * @param outBuffer A string that should be transmit to the other agent. NULL if there is no
 *                  data.
 * @return The length of the string pointed to by outBuffer (excludes \0 character).
 */
int AgentRun(char in, char *outBuffer)
{
    // State machine states

    static enum {
        INVALID,
        GENERATE_DATA,
        SEND_DATA,
        DETERMINE_ORDER,
        WAIT_FOR_GUESS,
        SEND_GUESS,
        WAIT_FOR_HIT,
        WON,
        LOST
    } AgentState = GENERATE_DATA;

    static FieldOledTurn whoseTurn;

    // State machine data
     static NegotiationData myNeg;
    static NegotiationData theirNeg;
     static GuessData myGuess;
    static  GuessData theirGuess;

    // non-static data
    int isParsed;
    int delayer;

    switch (AgentState) {
    case GENERATE_DATA:
        // Generates negotiation data and encodes challenge message to outBuffer
        ProtocolGenerateNegotiationData(&myNeg);
        AgentState = SEND_DATA;
        if (in == '$')
            ProtocolDecode('$', &theirNeg, &theirGuess);
        return ProtocolEncodeChaMessage(outBuffer, &myNeg);
        break;

    case SEND_DATA:
        // Parse next character if not null
        if (in)
            isParsed = ProtocolDecode(in, &theirNeg, &theirGuess);
        else
            isParsed = PROTOCOL_WAITING;

        // Advances state machine if parsed, fatal error if parsing fails
        if (isParsed == PROTOCOL_PARSED_CHA_MESSAGE) {
            AgentState = DETERMINE_ORDER;
            return ProtocolEncodeDetMessage(outBuffer, &myNeg);
        } else if (!(isParsed == PROTOCOL_WAITING || isParsed == PROTOCOL_PARSING_GOOD)) {
            OLED_PRINT_ERROR("parsing error, SEND_DATA");
        }

        return 0; //send nothing, wait for next char
        break;

    case DETERMINE_ORDER:
        // Parse next character if not null
        if (in)
            isParsed = ProtocolDecode(in, &theirNeg, &theirGuess);
        else
            isParsed = PROTOCOL_WAITING;

        // Advances state machine if parsed, fatal error if parsing fails
        if (isParsed == PROTOCOL_PARSED_DET_MESSAGE) {

            if (ProtocolValidateNegotiationData(&theirNeg)) {

                TurnOrder order = ProtocolGetTurnOrder(&myNeg, &theirNeg);

                if (order == TURN_ORDER_START) {
                    whoseTurn = FIELD_OLED_TURN_MINE; //sets turn to mine
                    // OledClear(OLED_COLOR_BLACK);
                    FieldOledDrawScreen(&localField, &enemyField, whoseTurn);
                    OledUpdate();

                    // Advance state and return 0
                    AgentState = SEND_GUESS;
                    return 0;
                } else if (order == TURN_ORDER_DEFER) {
                    whoseTurn = FIELD_OLED_TURN_THEIRS; //sets turn to theirs
                    // OledClear(OLED_COLOR_BLACK);
                    FieldOledDrawScreen(&localField, &enemyField, whoseTurn);
                    OledUpdate();

                    // Advance state and return 0
                    AgentState = WAIT_FOR_GUESS;
                    return 0;
                } else {
                    OLED_PRINT_ERROR("string ordering error");
                } // determines which agent goes first

            } else {
                OLED_PRINT_ERROR("negotiation data error");

            } // Evaluates whether negotiation data is valid

        } else if (!(isParsed == PROTOCOL_WAITING || isParsed == PROTOCOL_PARSING_GOOD)) {
            OLED_PRINT_ERROR("parsing error, DETERMINE_ORDER");
        }

        return 0; //send nothing, wait for next char
        break;

    case WAIT_FOR_GUESS:
        // Parse next character if not null
        if (in)
            isParsed = ProtocolDecode(in, &theirNeg, &theirGuess);
        else
            isParsed = PROTOCOL_WAITING;

        // Advances state machine if parsed, fatal error if parsing fails
        if (isParsed == PROTOCOL_PARSED_COO_MESSAGE) {

            FieldRegisterEnemyAttack(&localField, &theirGuess);

            if (AgentGetStatus() > 0) {
                AgentState = SEND_GUESS;
                whoseTurn = FIELD_OLED_TURN_MINE;
               //  OledClear(OLED_COLOR_BLACK);
                FieldOledDrawScreen(&localField, &enemyField, whoseTurn);
                OledUpdate();
                return ProtocolEncodeHitMessage(outBuffer, &theirGuess);
            } else {
               // printf("Dagim ")
                AgentState = LOST;
                whoseTurn = FIELD_OLED_TURN_NONE;
                // OledClear(OLED_COLOR_BLACK);
                FieldOledDrawScreen(&localField, &enemyField, whoseTurn);
                OledUpdate();
                return ProtocolEncodeHitMessage(outBuffer, &theirGuess);
            } //tells whether any boats remain

        } else if (!(isParsed == PROTOCOL_WAITING || isParsed == PROTOCOL_PARSING_GOOD)) {
            OLED_PRINT_ERROR("parsing error, WAIT_FOR_GUESS");
        }

        return 0; //send nothing, wait for next char
        break;

    case SEND_GUESS:

        // ADD DELAY HERE
        for (delayer = 0; delayer < 5000000; delayer++);

        GenerateGuessData(&myGuess);
        AgentState = WAIT_FOR_HIT;
        whoseTurn = FIELD_OLED_TURN_THEIRS;
        OledClear(OLED_COLOR_BLACK);
        FieldOledDrawScreen(&localField, &enemyField, whoseTurn);
        OledUpdate();
        return ProtocolEncodeCooMessage(outBuffer, &myGuess);
        break;

    case WAIT_FOR_HIT:
        // Parse next character if not null
        if (in)
            isParsed = ProtocolDecode(in, &theirNeg, &theirGuess);
        else
            isParsed = PROTOCOL_WAITING;

        // Advances state machine if parsed, fatal error if parsing fails
        if (isParsed == PROTOCOL_PARSED_HIT_MESSAGE) {

            FieldUpdateKnowledge(&enemyField, &theirGuess);

            if (FieldGetBoatStates(&enemyField)) {
                AgentState = WAIT_FOR_GUESS;
                whoseTurn = FIELD_OLED_TURN_THEIRS;
                // OledClear(OLED_COLOR_BLACK);
                FieldOledDrawScreen(&localField, &enemyField, whoseTurn);
                OledUpdate();
                return 0;
            } else {
                AgentState = WON;
                whoseTurn = FIELD_OLED_TURN_NONE;
               //  OledClear(OLED_COLOR_BLACK);
                FieldOledDrawScreen(&localField, &enemyField, whoseTurn);
                OledUpdate();
                return 0;
            } //tells whether any boats remain

        } else if (!(isParsed == PROTOCOL_WAITING || isParsed == PROTOCOL_PARSING_GOOD)) {
            OLED_PRINT_ERROR("parsing error, WAIT_FOR_HIT");
        }

        return 0; //send nothing, wait for next char
        break;

    case WON:
       // printf("win");
        break;

    case LOST:
        //printf("loss");
        break;
    }



    return STANDARD_ERROR;
}

/**
 * StateCheck() returns a 4-bit number indicating the status of that agent's ships. The smallest
 * ship, the 3-length one, is indicated by the 0th bit, the medium-length ship (4 tiles) is the
 * 1st bit, etc. until the 3rd bit is the biggest (6-tile) ship. This function is used within
 * main() to update the LEDs displaying each agents' ship status. This function is similar to
 * Field::FieldGetBoatStates().
 * @return A bitfield indicating the sunk/unsunk status of each ship under this agent's control.
 *
 * @see Field.h:FieldGetBoatStates()
 * @see Field.h:BoatStatus
 */
uint8_t AgentGetStatus(void)
{
    return FieldGetBoatStates(&localField);
}

/**
 * This function returns the same data as `AgentCheckState()`, but for the enemy agent.
 * @return A bitfield indicating the sunk/unsunk status of each ship under the enemy agent's
 *         control.
 *
 * @see Field.h:FieldGetBoatStates()
 * @see Field.h:BoatStatus
 */
uint8_t AgentGetEnemyStatus(void)
{
    return FieldGetBoatStates(&enemyField);
}

void GenerateGuessData(GuessData *data)
{
    do {
        data->col = (rand() % FIELD_COLS);
        data->row = (rand() % FIELD_ROWS);
    } while (enemyField.field[data->row][data->col] != FIELD_POSITION_UNKNOWN);
}