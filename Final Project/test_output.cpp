#include <iostream>

int main() {
	int p1, p2q, pr;
	p1 = 33;
	p2q = 412;
	pr = p1 + p2q;
	std::cout << pr << std::endl;
	pr = p1 * (p2q + 2 * pr);
	std::cout << "value=" << pr << std::endl;
	
	return 0;
}