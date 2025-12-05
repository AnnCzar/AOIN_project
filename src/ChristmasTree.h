class ChristmasTree{
    private:
        const double SCALE_FACTOR = 1e15;

        const double baseWidth = 0.7;
        const double midWidth = 0.4;
        const double peakWidth = 0.25;
        const double crownHeight = 0.8;
        const double trunkWidth = 0.15;
        const double trunkHeight = 0.2;
        const double trunkBottomY = - trunkHeight;
        const double baseY = 0.0;
        const double tier2Y = 0.25;
        const double tier1Y = 0.5;
        
        double x = 0.0;
        double y = 0.0;
        double angle;

    
    public:
        ChristmasTree(double x, double y, double angle);
    };
