

class TLC59401 {
  public:
    TLC59401(int mode, int blank, int gsClk, int xLat, int sClk, int sIn );
    void begin();
    void set( int channel, int amount);
    void blank( int blanked );
    void push();
  private:
    int _modePin;
    int _blankPin;
    int _gsClkPin;
    int _xLatPin;
    int _sClkPin;
    int _sInPin;

    int outputs[16] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
    
};
