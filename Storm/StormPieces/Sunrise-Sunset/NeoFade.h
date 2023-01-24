class NeoFade
{
public:
  void begin(void) 
  { 
    _period = 1000;
    reset(); 
    _ended = true;
  }
 
  inline void reset(void) 
  {
    setFade(_rStart, _gStart, _bStart,_wStart, _rEnd, _gEnd, _bEnd, _wEnd);
  }
 
  inline bool isEnded(void) { return(_ended); }
   
  inline void setPeriod(uint16_t p) { _period = p; calcTimeInterval(); }
  
  void setFade(const uint32_t color1, const uint32_t color2) {
  
  byte white1 = (color1 >> 24) & 0xff;
  byte red1 = (color1 >> 16) & 0xff;
  byte green1 = (color1 >> 8) & 0xff;
  byte blue1 = color1 & 0xff;

  byte white2 = (color2 >> 24) & 0xff;
  byte red2 = (color2 >> 16) & 0xff;
  byte green2 = (color2 >> 8) & 0xff;
  byte blue2 = color2 & 0xff;

  
  
  setFade(red1, green1, blue1, white1, red2, green2, blue2, white2);
  }

  void setFade(uint16_t r0, uint16_t g0, uint16_t b0, uint16_t w0, uint16_t r1, uint16_t g1, uint16_t b1, uint16_t w1)
    {
      _rStart = _r0 = r0;  _rEnd = _r1 = r1;
      _gStart = _g0 = g0;  _gEnd = _g1 = g1;
      _bStart = _b0 = b0;  _bEnd = _b1 = b1;
      _wStart = _w0 = w0;  _wEnd = _w1 = w1;
       
      _dR = uiabs(_r1,_r0);  _sR = _r0 < _r1 ? 1 : -1;
      _dG = uiabs(_g1,_g0);  _sG = _g0 < _g1 ? 1 : -1;
      _dB = uiabs(_b1,_b0);  _sB = _b0 < _b1 ? 1 : -1;
      _dW = uiabs(_w1,_w0);  _sW = _w0 < _w1 ? 1 : -1;  
          
      _dMax = max4(_dR, _dG, _dB, _dW);
      _steps = _dMax;
      calcTimeInterval();
      _timeStart = 0;
 
      _r1 = _g1 = _b1 = _dMax/2;            /* error offset */
 
      _ended = false;
    }
 
  bool getNext(uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &w)
  {
    if (millis() - _timeStart < _timeInterval)
      return(false);
 
    _timeStart = millis();
 
    r = _r0;
    g = _g0;
    b = _b0;
    w = _w0;
    
    _ended = (_steps == 0);
    if (!_ended)
    {
      _steps--;
      _r1 -= _dR; if (_r1 < 0) { _r1 += _dMax; _r0 += _sR; }
      _g1 -= _dG; if (_g1 < 0) { _g1 += _dMax; _g0 += _sG; }
      _b1 -= _dB; if (_b1 < 0) { _b1 += _dMax; _b0 += _sB; }
      _w1 -= _dW; if (_w1 < 0) { _w1 += _dMax; _w0 += _sW; }
    }
 
    return(true);
  }
 
private:
  uint16_t _rStart, _gStart, _bStart, _wStart, _rEnd, _gEnd, _bEnd, _wEnd;
  int32_t _r0, _g0, _b0, _w0, _r1, _g1, _b1, _w1, _steps;
  int16_t _dR, _dG, _dB, _dW, _dMax;
  int8_t _sR, _sG, _sB, _sW;
  bool _ended;
 
  // timing parameters
  uint16_t _period;    // fade duration in ms
  uint32_t _timeStart; // animation time start
  uint32_t _timeInterval; // interval between color changes
   
  void calcTimeInterval(void)
  {
    _timeInterval = _period / _steps;
    if (_timeInterval == 0) _timeInterval = 1;
  }
 
  uint16_t max3(uint16_t x, uint16_t y, uint16_t z)
  {
    uint16_t m = z;    // assume z to start with
     
    if (x > y) { if (x > z) m = x; }
    else { if (y > z) m = y; }
     
    return(m);
  }

  uint16_t max4(uint16_t w, uint16_t x, uint16_t y, uint16_t z)
  {
    uint16_t m = z;    // assume z to start with
    if (w > x) {if (w > y){if (w > z)m = w;}} 
    if (x > w) {if (x > y){if (x > z)m = x;}} 
    if (y > w) {if (y > x){if (y > z)m = y;}} 
     
    return(m);
  }
 
  inline uint16_t uiabs(uint16_t a, uint16_t b) { return((a > b) ? (a - b) : (b - a)); }
 
};
