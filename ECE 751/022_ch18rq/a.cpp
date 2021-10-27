class a
{
private:
	int data;
public:
	a():data(0){}
	a(int x):data(x){}	
};

class b : public a
{
	a():(4){}
};

int main(){
	a ana;
	b bnb;
	return 0;
}