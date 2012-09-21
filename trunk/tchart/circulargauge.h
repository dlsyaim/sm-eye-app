// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


// Dispatch interfaces referenced by this interface
class CTeeShape;
class CFramedBorder;
class CGaugePointerRange;
class CGaugeSeriesPointer;
class CPointer;

/////////////////////////////////////////////////////////////////////////////
// CCircularGauge wrapper class

class CCircularGauge : public COleDispatchDriver
{
public:
	CCircularGauge() {}		// Calls COleDispatchDriver default constructor
	CCircularGauge(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CCircularGauge(const CCircularGauge& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CTeeShape GetFace();
	CFramedBorder GetFrame();
	double GetValue();
	void SetValue(double newValue);
	CGaugePointerRange GetGreenLine();
	BOOL GetHorizontal();
	void SetHorizontal(BOOL bNewValue);
	double GetMaximum();
	void SetMaximum(double newValue);
	double GetMinimum();
	void SetMinimum(double newValue);
	long GetMinorTickDistance();
	void SetMinorTickDistance(long nNewValue);
	CGaugeSeriesPointer GetMinorTicks();
	CGaugePointerRange GetRedLine();
	CGaugeSeriesPointer GetTicks();
	CGaugeSeriesPointer GetCenter();
	BOOL GetCircled();
	void SetCircled(BOOL bNewValue);
	CPointer GetEndPoint();
	BOOL GetLabelsInside();
	void SetLabelsInside(BOOL bNewValue);
	BOOL GetRotateLabels();
	void SetRotateLabels(BOOL bNewValue);
	double GetRotationAngle();
	void SetRotationAngle(double newValue);
	double GetTotalAngle();
	void SetTotalAngle(double newValue);
	CGaugeSeriesPointer GetHand();
};
