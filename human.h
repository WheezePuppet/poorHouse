class human
{
public:
	human();
	void earnIncome(float);
	void consume();
	void haveChildren();
	void die();
	~human();
private:
	float mps;
	float salary;
	vector<float> cons;
	float savings;
	int age;
	human *parent;
	human *children;
};
