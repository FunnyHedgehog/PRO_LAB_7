#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h> 
#include <iostream>
#include <string>
#include <time.h>
#include <chrono>
#include <math.h>
#include <cilk/reducer.h>
#include <cilk/cilk_api.h>
using namespace std;

class Timer
{
public:
	void Start()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	void Stop()
	{
		end = std::chrono::high_resolution_clock::now();
	}
	double elapsedMilliseconds()
	{
		duration = end - start;
		return duration.count();
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float, milli> duration;
};

struct Point {
	double x, y;
};

double D(Point X)
{
	return (X.x - X.y) / 2;
}
Point T(Point X)
{
	Point X1;
	X1.x = 2 * D(X) - X.x;
	X1.y = -2 * D(X) - X.y;
	return X1;
}

int main()
{
	Point points[3], pointsReversed[3];
	Timer time;
	int num = 3;
	cout << "Enter triangle vertices:\n";
	for (int i = 0; i < num; ++i) {
		cout << "X" << i + 1 << " : ";
		cin >> points[i].x;
		cout << "Y" << i + 1 << " : ";
		cin >> points[i].y;
	}
	
	time.Start();
	for (int i = 0; i < num; i++)
	{
		pointsReversed[i] =	T(points[i]);
	}
	time.Stop();
	cout << "\nMono-thread time: " << time.elapsedMilliseconds() <<endl;
	time.Start();
	__cilkrts_set_param("nworkers", "3");
	cilk_for(int i = 0; i < num; i++)
	{
		pointsReversed[i] = T(points[i]);
	}
	time.Stop();
	cout << "Parallel time: " << time.elapsedMilliseconds() <<endl;
	cout << "New triangle vertices: " << endl;
	for (int i = 0; i < num; i++)
	{
		cout << "X" << i + 1 << " : " << pointsReversed[i].x <<endl<< "Y" << i + 1 << " : " << pointsReversed[i].y << endl;
	}

}