#include <iostream>
#include <queue>
#include <algorithm>
#include <iomanip>

struct patient
{
	int number;
	// times in minutes
	int arrival, hospitalization, left, serve_time;
	int bed_number;
	bool alive = true;
} temp;

const bool FCFScompare(const patient& first, const patient& second)
{
	return first.arrival < second.arrival;
}

std::queue<patient> FCFS(patient* const patients, const int& number_of_patients)
{
	std::sort(patients, patients + number_of_patients, FCFScompare);
	std::queue<patient> patients_in_queue;
	for(int i = 0; i < number_of_patients; ++i)
	{
		patients_in_queue.push(patients[i]);
	}
	return patients_in_queue;
}

void hospitalize(std::queue<patient> patients, const int& number_of_beds)
{
	int number_of_patients = patients.size();
	long long time = 0;
	int alive = 0, dead = 0;
	float average_wait_time;
	// stores times at which each bed is free
	int beds[number_of_beds];
	for(int i = 0; i < number_of_beds; i++)
	{
		beds[i] = 0;
	}
	bool served;
	while (!patients.empty())
	{
		temp = patients.front();
		patients.pop();
		served = false;
		while (temp.alive && !served)
		{
			for (int i = 0; i < number_of_beds && !served; ++i)
			{
				if (beds[i] <= time)
				{
					std::cout << "patient " << temp.number << " entered bed " << i + 1 << " at " << time << " and left at " << time + temp.hospitalization<< '\n';
					beds[i] = beds[i] += temp.hospitalization;
					served = true;
					++alive;
				}
			}
			if (!served)
			{
				++time;
				++average_wait_time;
			}
			if (time > temp.arrival + temp.left)
			{
				temp.alive = false;
				std::cout << "patient " << temp.number << " died at " << time << '\n';
				++dead;
			}
		}
	}
	std::cout << "\nfinal results:\nlive patients: " << alive << "\ndead patients: " << dead << std::fixed << std::setprecision(2) << "\naverage wait time: " << average_wait_time / number_of_patients << '\n';
}

int main()
{
	int beds;
	std::cout << "enter number of beds --> ";
	std::cin >> beds;
	int number_of_patients;
	std::cout << "enter number of patients --> ";
	std::cin >> number_of_patients;
	patient patients[number_of_patients];
	std::cout << "enter patients' information: <number> <arrival> <hospitalization> <left>\n";
	for (int i = 0; i < number_of_patients; ++i)
	{
		std::cin >> temp.number >> temp.arrival >> temp.hospitalization >> temp.left;
		patients[i] = temp;
	}
	std::queue<patient> patients_in_queue = FCFS(patients, number_of_patients);
	std::cout << "FCFS scenario:\n";
	hospitalize(patients_in_queue, beds);
	return EXIT_SUCCESS;
}
