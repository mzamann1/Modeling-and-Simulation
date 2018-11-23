#include "stdio.h"
#include "stdlib.h"
#include "math.h"

#define Q_LIMIT 100
#define BUSY    1
#define IDLE    0

int next_event_type, num_custs_delayed, num_delays_required, num_events, num_in_q, server_status, stopping_cond;

float area_num_in_q, area_server_status, mean_interarrival, mean_service, time, time_arrival[Q_LIMIT + 1], time_last_event, time_next_event[3], total_of_delays;

FILE *infile, *outfile;

void initialize(void);
void timing(void);
void arrive(void);
void depart(void);
void report(void);
void update_time_avg_status(void);
float expon(float mean);



int main(int argc, char const *argv[]) {
	//infile =fopen("mm1.in","r");
	infile = fopen("mm1.in", "r");
	outfile = fopen("mm1.out", "w");

	num_events = 2;

	fscanf(infile, "%f %f %d", &mean_interarrival, &mean_service, &num_delays_required); //&num_delays_required
	printf("%d\n", num_delays_required);
	fprintf(outfile, "Single-Server Queing System\n\n");
	fprintf(outfile, "Mean InterArrival Time: %.f minutes\n\n", mean_interarrival);
	fprintf(outfile, "Mean Serrvice Time: %.3f minutes\n\n", mean_service);
	fprintf(outfile, "Number of Customers: %d\n\n", num_delays_required);
	//fprintf(outfile, "Stopping Condition %d\n", stopping_cond);
	for (int i = 1; i <= 10; i++) {
	  /* code */


	initialize();

	while (num_custs_delayed < num_delays_required)
		//while (time < stopping_cond)
	{
		timing();
		update_time_avg_status();

		switch (next_event_type)
		{
		case 1:
			arrive();
			break;
		case 2:
			depart();
			break;
		}
	}
	report();
	}
	fclose(infile);
	fclose(outfile);
	getchar();


	return 0;
}



void initialize(void)
{
	time = 0.0;

	server_status = IDLE;
	num_in_q = 0;
	time_last_event = 0.0;


	num_custs_delayed = 0.0;
	total_of_delays = 0.0;
	area_num_in_q = 0.0;
	area_server_status = 0.0;

	time_next_event[1] = time + expon(mean_interarrival);
	time_next_event[2] = 1.0e+30;

}


void timing(void)
{
	int i;
	float min_time_next_event = 1.0e+29;

	next_event_type = 0;

	for (i = 1; i <= num_events; ++i)
	{
		if (time_next_event[i] < min_time_next_event)
		{
			min_time_next_event = time_next_event[i];
			next_event_type = i;
		}
	}

	if (next_event_type == 0)
	{
		fprintf(outfile, "Event List Empty at time %f\n", time);
		exit(1);
	}
	time = min_time_next_event;
}

void arrive(void)
{
	float delay;

	time_next_event[1] = time + expon(mean_interarrival);

	if (server_status == BUSY)
	{
		++num_in_q;

		if (num_in_q > Q_LIMIT)
		{
			fprintf(outfile, "OverFlow at time %f", time);
			exit(2);
		}
		time_arrival[num_in_q] = time;
	}
	else
	{
		delay = 0.0;
		total_of_delays += delay;

		++num_custs_delayed;
		server_status = BUSY;

		time_next_event[2] = time + expon(mean_service);
	}
}

void depart(void)
{
	int i;
	float delay;

	if (num_in_q == 0)
	{
		server_status = IDLE;
		time_next_event[2] = 1.0e+30;

	}
	else
	{
		--num_in_q;

		delay = time - time_arrival[1];
		total_of_delays += delay;

		++num_custs_delayed;
		time_next_event[2] = time + expon(mean_service);

		for (i = 1; i < num_in_q; ++i)
		{
			time_arrival[i] = time_arrival[i + 1];
		}
	}
}

void report(void)
{
	fprintf(outfile, "\nAverage Delay in Queue : %11.3f minutes\n\n", total_of_delays / num_custs_delayed);
	fprintf(outfile, "\nAverage Num in Queue : %10.3f\n\n", area_num_in_q / time);
	fprintf(outfile, "Server Utilization : %15.3f\n\n", area_server_status / time);
	fprintf(outfile, "Time Simulation Ended : %12.3f\n\n\n\n", time);

}

void update_time_avg_status(void)
{
	float time_since_last_event;

	time_since_last_event = time - time_last_event;
	time_last_event = time;

	area_num_in_q += num_in_q * time_since_last_event;

	area_server_status += server_status * time_since_last_event;
}

float  expon(float mean)
{
	float u;
	u = rand() % 500 + 1;

	return mean * log(u);
}
