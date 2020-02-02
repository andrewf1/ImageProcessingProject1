#ifndef ROI_HPP
#define ROI_HPP

// #include "../iptools/core.h" 

class roi 
{
public:
    roi() = default;
    roi(int x_i, int y_i, int sx_i, int sy_i, 
        int T1_i, int T2_i,
        int cR_i, int cG_i, int cB_i, int TC_i, int DC_i,
        int WS_i) {
            // img_in = img_src;
            x = x_i;
            y = y_i;
            sx = sx_i;
            sy = sy_i;
            T1 = T1_i;
            T2 = T2_i;
            cR = cR_i;
            cG = cG_i;
            cB = cB_i;
            TC = TC_i;
            DC = DC_i;
            WS = WS_i;
        };
    ~roi() = default;
    
    // image img_in, img_out;
    int x;
    int y;
    int sx;
    int sy;
    int T1, T2;
    int cR, cG, cB, TC, DC;
    int WS;
};

#endif