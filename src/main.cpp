#include <iostream> // cout, cin
#include <queue> // queue
#include <algorithm> // sort()
#include <iomanip> // setprecision()

using namespace std;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"

struct patient
{
	int number;
	int arrival, hospitalization, left, serve_time; // times - in minutes
	bool served_at_arrival = false;
	bool alive = true;
} temp;

// first come, first served (FCFS) scenario
const bool FCFScompare(const patient& first, const patient& second)
{
	return first.arrival < second.arrival;
}

queue<patient> FCFS(patient* const patients, const int& number_of_patients)
{
	// sorting based on arrival times
	sort(patients, patients + number_of_patients, FCFScompare);
	// placing in queue
	queue<patient> patients_in_queue;
	for(int i = 0; i < number_of_patients; ++i)
	{
		patients_in_queue.push(patients[i]);
	}
	return patients_in_queue;
}
// end of FCFS scenario

// shortest job first (SJF) scenario
queue<patient> SJF(patient* const patients, const int& number_of_patients, const int& number_of_beds)
{
	// stores last patient's hospitalization duration
	int hos_time;
	// true if two elements were swapped (for better performance)
	bool swapped = true;
	int beds[number_of_beds];
	for (int i = 0; i < number_of_beds; ++i)
	{
		beds[i] = 0;
	}
	// checking if patients get served as soon as they arrive
	for (int i = 0; i < number_of_patients; ++i)
	{
		for (int j = 0; j < number_of_beds; ++j)
			if (beds[j] <= patients[i].arrival)
			{
				beds[j] += patients[i].hospitalization;
				patients[i].served_at_arrival = true;
				break;
			}
	}
	// bubble sort
	for (int i = 0; i < number_of_patients - 1 && swapped; ++i)
	{
		hos_time = 0;
		swapped = false;
		for (int j = 0; j < number_of_patients - i - 1; ++j)
		{
			// lower priority patient was served at
			if (!(patients[j].arrival != patients[j + 1].arrival && patients[j].served_at_arrival)
			   && (patients[j].arrival <= hos_time && patients[j + 1].arrival <= hos_time) // two patients are waiting for an empty bed
			   && patients[j].hospitalization > patients[j + 1].hospitalization) // main condition
			{
				// swapping process
				temp = patients[j];
				patients[j] = patients[j + 1];
				patients[j + 1] = temp;
				swapped = true;
			}
			// updating hos_time
			hos_time = patients[j].arrival + patients[j].hospitalization;
		}
	}
	queue<patient> patients_in_queue;
	for(int i = 0; i < number_of_patients; ++i)
	{
		patients_in_queue.push(patients[i]);
	}
	return patients_in_queue;

}
// end of SJF scenario

// priority scheduling (PS) scenario
queue<patient> PS(patient* const patients, const int& number_of_patients, const int& number_of_beds)
{
	int hos_time;
	bool swapped = true;
	int beds[number_of_beds];
	for (int i = 0; i < number_of_beds; ++i)
	{
		beds[i] = 0;
	}
	for (int i = 0; i < number_of_patients; ++i)
	{
		for (int j = 0; j < number_of_beds; ++j)
			if (beds[j] <= patients[i].arrival)
			{
				beds[j] += patients[i].hospitalization;
				patients[i].served_at_arrival = true;
				break;
			}
	}
	for (int i = 0; i < number_of_patients - 1 && swapped; ++i)
	{
		hos_time = 0;
		swapped = false;
		for (int j = 0; j < number_of_patients - i - 1; ++j)
		{
			if (!(patients[j].arrival != patients[j + 1].arrival && patients[j].served_at_arrival)
			   && (patients[j].arrival <= hos_time && patients[j + 1].arrival <= hos_time)
			   && patients[j].left > patients[j + 1].left) // main swapping condition changes compared to SJF
			{
				temp = patients[j];
				patients[j] = patients[j + 1];
				patients[j + 1] = temp;
				swapped = true;
			}
			hos_time = patients[j].arrival + patients[j].hospitalization;
		}
	}
	queue<patient> patients_in_queue;
	for(int i = 0; i < number_of_patients; ++i)
	{
		patients_in_queue.push(patients[i]);
	}
	return patients_in_queue;

}
// end of PS scenario

// serves patients and prints results
void hospitalize(queue<patient> patients, const int& number_of_beds)
{
	int number_of_patients = patients.size();
	// keeps track of time (in minutes)
	long long time = 0;
	// dead and live patients
	int alive = 0, dead = 0;
	float average_wait_time = 0;
	// stores times at which each bed is free
	int beds[number_of_beds];
	for(int i = 0; i < number_of_beds; ++i)
	{
		// all beds are free at first
		beds[i] = 0;
	}
	// true if current patient gets a bed
	bool served;
	while (!patients.empty())
	{
		// serving first in line
		temp = patients.front();
		patients.pop();
		served = false;
		while (temp.alive && !served)
		{
			// checking each bed's availability
			for (int i = 0; i < number_of_beds && !served; ++i)
			{
				if (beds[i] <= time)
				{
					// printing entry and exit times
					cout << ANSI_COLOR_BLUE "patient " << temp.number << " entered bed " << i + 1 << " at " << time
					          << " and left at " << time + temp.hospitalization << '\n' << ANSI_COLOR_RESET;
					// updating bed's availability time
					beds[i] = beds[i] += temp.hospitalization;
					served = true;
					// live patient increase
					++alive;
				}
			}
			// patient must wait to be served
			if (!served)
			{
				++time;
				++average_wait_time;
			}
			// patient died before being served
			if (time > temp.arrival + temp.left)
			{
				temp.alive = false;
				cout << "patient " << temp.number << " died at " << time << '\n';
				++dead;
			}
		}
	}
	// printing average wait time and the number of live and dead people
	cout << ANSI_COLOR_MAGENTA "\n---final results---\n" ANSI_COLOR_RESET << "live patients: " << ANSI_COLOR_GREEN << alive << ANSI_COLOR_RESET "\ndead patients: " <<  ANSI_COLOR_RED << dead <<  ANSI_COLOR_RESET
	          << fixed << setprecision(2) << "\naverage wait time: " << average_wait_time / number_of_patients << '\n';
}

int main()
{
	int beds;
	cout << "enter number of beds --> ";
	cin >> beds;
	int number_of_patients;
	cout << "enter number of patients --> ";
	cin >> number_of_patients;
	patient patients[number_of_patients];
	cout << "enter patients' information:" << ANSI_COLOR_YELLOW " <number> <arrival> <hospitalization> <left>\n" ANSI_COLOR_RESET;
	// storing patients in an array
	for (int i = 0; i < number_of_patients; ++i)
	{
		cin >> temp.number >> temp.arrival >> temp.hospitalization >> temp.left;
		patients[i] = temp;
	}
	queue<patient> patients_in_queue = FCFS(patients, number_of_patients);
	cout << ANSI_COLOR_YELLOW "\nFCFS scenario:\n" ANSI_COLOR_RESET;
	hospitalize(patients_in_queue, beds);
	cout << ANSI_COLOR_MAGENTA "---------------------------------------------\n" ANSI_COLOR_RESET;
	cout << ANSI_COLOR_YELLOW "SJF scenario:\n" ANSI_COLOR_RESET;
	patients_in_queue = SJF(patients, number_of_patients, beds);
	hospitalize(patients_in_queue, beds);
	cout << ANSI_COLOR_MAGENTA "---------------------------------------------\n" ANSI_COLOR_RESET;
	cout << ANSI_COLOR_YELLOW "priority scheduling scenario:\n" ANSI_COLOR_RESET;
	patients_in_queue = PS(patients, number_of_patients, beds);
	hospitalize(patients_in_queue, beds);
	
	return EXIT_SUCCESS;
}