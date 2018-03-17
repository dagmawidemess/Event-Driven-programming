#include "Field.h"
#include "Oled.h"
#include "FieldOled.h"
#include "Protocol.h"

static int x, y;

void FieldInit(Field *f, FieldPosition p)
{

    for (x = 0; x < FIELD_ROWS; x++) {
        for (y = 0; y < FIELD_COLS; y++) {
            f->field[x][y] = p;
        }
    }
    f->hugeBoatLives = FIELD_BOAT_LIVES_HUGE;
    f->largeBoatLives = FIELD_BOAT_LIVES_LARGE;
    f->smallBoatLives = FIELD_BOAT_LIVES_SMALL;
    f->mediumBoatLives = FIELD_BOAT_LIVES_MEDIUM;
}

/**
 * Retrieves the value at the specified field position.
 * @param f The Field being referenced
 * @param row The row-component of the location to retrieve
 * @param col The column-component of the location to retrieve
 * @return
 */
FieldPosition FieldAt(const Field *f, uint8_t row, uint8_t col)
{
    if (f) {
        return f->field[row][col];
    } else {
        return STANDARD_ERROR;
    }
}

/**
 * This function provides an interface for setting individual locations within a Field struct. This
 * is useful when FieldAddBoat() doesn't do exactly what you need. For example, if you'd like to use
 * FIELD_POSITION_CURSOR, this is the function to use.
 * 
 * @param f The Field to modify.
 * @param row The row-component of the location to modify
 * @param col The column-component of the location to modify
 * @param p The new value of the field location
 * @return The old value at that field location
 */
FieldPosition FieldSetLocation(Field *f, uint8_t row, uint8_t col, FieldPosition p)
{
    FieldPosition lastPosition = f->field[row][col];
    f->field[row][col] = p;
    return lastPosition;

}

/**
 * FieldAddBoat() places a single ship on the player's field based on arguments 2-5. Arguments 2, 3
 * represent the x, y coordinates of the pivot point of the ship.  Argument 4 represents the
 * direction of the ship, and argument 5 is the length of the ship being placed. All spaces that
 * the boat would occupy are checked to be clear before the field is modified so that if the boat
 * can fit in the desired position, the field is modified as SUCCESS is returned. Otherwise the
 * field is unmodified and STANDARD_ERROR is returned. There is no hard-coded limit to how many
 * times a boat can be added to a field within this function.
 *
 * So this is valid test code:
 * {
 *   Field myField;
 *   FieldInit(&myField,FIELD_POSITION_EMPTY);
 *   FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_SMALL);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_MEDIUM);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_HUGE);
 *   FieldAddBoat(&myField, 0, 6, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_SMALL);
 * }
 *
 * should result in a field like:
 *  _ _ _ _ _ _ _ _
 * [ 3 3 3       3 ]
 * [ 4 4 4 4     3 ]
 * [             3 ]
 *  . . . . . . . .
 *
 * @param f The field to grab data from.
 * @param row The row that the boat will start from, valid range is from 0 and to FIELD_ROWS - 1.
 * @param col The column that the boat will start from, valid range is from 0 and to FIELD_COLS - 1.
 * @param dir The direction that the boat will face once places, from the BoatDirection enum.
 * @param boatType The type of boat to place. Relies on the FIELD_POSITION_*_BOAT values from the
 * FieldPosition enum.
 * @return TRUE for success, FALSE for failure
 */
uint8_t FieldAddBoat(Field *f, uint8_t row, uint8_t col, BoatDirection dir, BoatType type)
{
    int boatLength;
    FieldPosition fieldPosition;

    switch (type) {
    case(FIELD_BOAT_SMALL):
        boatLength = FIELD_BOAT_LIVES_SMALL;
        fieldPosition = FIELD_POSITION_SMALL_BOAT;
        break;
    case(FIELD_BOAT_MEDIUM):
        boatLength = FIELD_BOAT_LIVES_MEDIUM;
        fieldPosition = FIELD_POSITION_MEDIUM_BOAT;
        break;
    case(FIELD_BOAT_LARGE):
        boatLength = FIELD_BOAT_LIVES_LARGE;
        fieldPosition = FIELD_POSITION_LARGE_BOAT;
        break;
    case(FIELD_BOAT_HUGE):
        boatLength = FIELD_BOAT_LIVES_HUGE;
        fieldPosition = FIELD_POSITION_HUGE_BOAT;
        break;
    }

    int newCol = col;
    int newRow = row;

    while ((newCol >= 0) && (newRow >= 0) && (newRow < FIELD_ROWS) && (newCol < FIELD_COLS) &&
            (abs(newCol - col) < boatLength) && (abs(newRow - row) < boatLength)) {

        if (f->field[newRow][newCol] != FIELD_POSITION_EMPTY) {
            return FALSE;
        }
        switch (dir) {
        case FIELD_BOAT_DIRECTION_NORTH:
            newRow--;
            break;
        case FIELD_BOAT_DIRECTION_SOUTH:
            newRow++;
            break;
        case FIELD_BOAT_DIRECTION_EAST:
            newCol++;
            break;
        case FIELD_BOAT_DIRECTION_WEST:
            newCol--;
            break;
        }
    }

    if ((abs(newCol - col) != boatLength) && (abs(newRow - row) != boatLength)) {
        return FALSE;
    }

    newCol = col;
    newRow = row;

    while ((newCol >= 0) && (newRow >= 0) && (newRow < FIELD_ROWS) && (newCol < FIELD_COLS) &&
            (abs(newCol - col) < boatLength) && (abs(newRow - row) < boatLength)) {

        f->field[newRow][newCol] = fieldPosition;
        switch (dir) {
        case FIELD_BOAT_DIRECTION_NORTH:
            newRow--;
            break;
        case FIELD_BOAT_DIRECTION_SOUTH:
            newRow++;
            break;
        case FIELD_BOAT_DIRECTION_EAST:
            newCol++;
            break;
        case FIELD_BOAT_DIRECTION_WEST:
            newCol--;
            break;
        }
    }

    switch (type) {
    case(FIELD_BOAT_SMALL):
        f->smallBoatLives = FIELD_BOAT_LIVES_SMALL;
        break;
    case(FIELD_BOAT_MEDIUM):
        f->mediumBoatLives = FIELD_BOAT_LIVES_MEDIUM;
        break;
    case(FIELD_BOAT_LARGE):
        f->largeBoatLives = FIELD_BOAT_LIVES_LARGE;
        break;
    case(FIELD_BOAT_HUGE):
        f->hugeBoatLives = FIELD_BOAT_LIVES_HUGE;
        break;
    }

    return TRUE;

}

/**
 * This function registers an attack at the gData coordinates on the provided field. This means that
 * 'f' is updated with a FIELD_POSITION_HIT or FIELD_POSITION_MISS depending on what was at the
 * coordinates indicated in 'gData'. 'gData' is also updated with the proper HitStatus value
 * depending on what happened AND the value of that field position BEFORE it was attacked. Finally
 * this function also reduces the lives for any boat that was hit from this attack.
 * @param f The field to check against and update.
 * @param gData The coordinates that were guessed. The HIT result is stored in gData->hit as an
 *               output.
 * @return The data that was stored at the field position indicated by gData before this attack.
 */
FieldPosition FieldRegisterEnemyAttack(Field *f, GuessData *gData)
{
    FieldPosition fieldPosition = f->field[gData->row][gData->col];

    switch (fieldPosition) {
    case FIELD_POSITION_SMALL_BOAT:
        f->smallBoatLives--;//decrement if its not a sink
        if (f->smallBoatLives == 0) {
            gData->hit = HIT_SUNK_SMALL_BOAT;
        } else {
            gData->hit = HIT_HIT;
        }

        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        break;
    case FIELD_POSITION_MEDIUM_BOAT:
        f->mediumBoatLives--;//decrement if its not a sink
        if (f->mediumBoatLives == 0) {
            gData->hit = HIT_SUNK_MEDIUM_BOAT;
        } else {
            gData->hit = HIT_HIT;
        }

        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        break;
    case FIELD_POSITION_LARGE_BOAT:
        f->largeBoatLives--;//decrement if its not a sink
        if (f->largeBoatLives == 0) {
            gData->hit = HIT_SUNK_LARGE_BOAT;
        } else {
            gData->hit = HIT_HIT;
        }

        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        break;
    case FIELD_POSITION_HUGE_BOAT:
        f->hugeBoatLives--;//decrement if its not a sink
        if (f->hugeBoatLives == 0) {
            gData->hit = HIT_SUNK_HUGE_BOAT;
        } else {
            gData->hit = HIT_HIT;
        }

        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        break;
    case FIELD_POSITION_HIT:
        gData->hit = HIT_HIT;
        break;
    default:
        gData->hit = HIT_MISS;
        f->field[gData->row][gData->col] = FIELD_POSITION_MISS;
        break;
    }

    return fieldPosition;

}

/**
 * This function updates the FieldState representing the opponent's game board with whether the
 * guess indicated within gData was a hit or not. If it was a hit, then the field is updated with a
 * FIELD_POSITION_HIT at that position. If it was a miss, display a FIELD_POSITION_EMPTY instead, as
 * it is now known that there was no boat there. The FieldState struct also contains data on how
 * many lives each ship has. Each hit only reports if it was a hit on any boat or if a specific boat
 * was sunk, this function also clears a boats lives if it detects that the hit was a
 * HIT_SUNK_*_BOAT.
 * @param f The field to grab data from.
 * @param gData The coordinates that were guessed along with their HitStatus.
 * @return The previous value of that coordinate position in the field before the hit/miss was
 * registered.
 */
FieldPosition FieldUpdateKnowledge(Field *f, const GuessData *gData)
{
    FieldPosition fieldPosition = f->field[gData->row][gData->col];

    switch (gData->hit) {
        //when hit to boat happens
    case HIT_HIT:
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        break;
        //when a hit is missed
    case HIT_MISS:
        f->field[gData->row][gData->col] = FIELD_POSITION_EMPTY;
        break;
        //when all the boats are sunked
    case HIT_SUNK_SMALL_BOAT:
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        f->smallBoatLives = 0;
        break;
    case HIT_SUNK_MEDIUM_BOAT:
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        f->mediumBoatLives = 0;
        break;
    case HIT_SUNK_LARGE_BOAT:
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        f->largeBoatLives = 0;
        break;
    case HIT_SUNK_HUGE_BOAT:
        f->field[gData->row][gData->col] = FIELD_POSITION_HIT;
        f->hugeBoatLives = 0;
        break;
    }

    return fieldPosition;
}

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field *f)
{

    uint8_t boatStates = 0; //1-alive 0-dead

    if (f->hugeBoatLives > 0) {
        boatStates |= FIELD_BOAT_STATUS_HUGE;
    }

    if (f->largeBoatLives > 0) {
        boatStates |= FIELD_BOAT_STATUS_LARGE;
    }

    if (f->mediumBoatLives > 0) {
        boatStates |= FIELD_BOAT_STATUS_MEDIUM;
    }

    if (f->smallBoatLives > 0) {
        boatStates |= FIELD_BOAT_STATUS_SMALL;
    }

    return boatStates; //return results of BoatStates

}