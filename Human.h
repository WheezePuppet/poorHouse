class Human
{
public:
	Human();
	void earnIncome(float);
	void consume();
	void haveChildren();
	void die();
	~Human();
private:
	float mps;
	float salary;
	vector<float> cons;
	float savings;
	int age;
	Human *parent;
	vector<human *> children;
};
