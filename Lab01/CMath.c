#include "CMath.h"
float myabs(float x);




float myabs(float x)
{
    if(x<0){
        x=x*-1;
    } else{
        return x;
   }
   
}
float enorm(float px,float py, float qx,float qy);




float enorm(float px,float py, float qx,float qy)
{
            float DX =qx-px;
            float DY = qy-py;
            float g; 
            float e;
            float t;
            float r;
            float s; 
            float l;
    DX =myabs(DX);
    DY =myabs(DY);
    if (DX>DY){
        g=DX;
        e=DY;
}else{
        g=DY;
        e=DX;
} while(l<2){
    t=e/g;
    r=t*t;
    s=r/(4.0+r);
    g=g+2*s*g;
    e=e*s;
    l=l+1;
    return g;
            
}
    }
    
float arctangent2(float y, float x);



float arctangent2(float y, float x)
{
    float n;
    float m;
    float noty = myabs(y);
    if (x>0) {
        n=(x-noty)/(x+noty);
        m=((0.1963*n*n*n)-(0.9817*n)+(3.141593/4.0));
        
    }else{
        n=(x+noty)/(x-noty);
        m=(0.1963*n*n*n)-(0.9817*n)+((3.0*3.141593)/4.0);
       
    }
    if (y<0){
        m=m*-1;
        
    }
    return m;
}

