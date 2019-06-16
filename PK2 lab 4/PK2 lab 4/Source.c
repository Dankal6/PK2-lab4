#include <stdlib.h>
#include <stdio.h>

//jednostki moga byc dowolne, najlepiej inty, ekspres moze sie uzupelniac sam (smart ekspres), lub na polecenie uzytkownika, stany zczytywane scanf
//do projektu- podzial na funkcje robienia czegos madrego, oraz funkcje te wyniki wyswietlajace
//sprawdzanie poprawnosci (np moc kawy 1-10, jak ktos wpisze a nie ma sie wywalac)

typedef enum { STATE_INITIAL, STATE_SMALL_COFFEE, STATE_BIG_COFFEE, STATE_ESPRESSO, STATE_CAPPUCINO, STATE_HOT_MILK, STATE_DEF_POWER, STATE_AMOUNT_OF_WATER, NUM_STATES } _states;

typedef struct {
	int amount_of_water;
	int amount_of_milk;
	int amount_of_coffee;
	int power;
	int base_amount_of_water;
}_ekspres;

/*typedef _states state_fun(_ekspres *ekspres);

_states do_small_coffee(_ekspres *ekspres);
_states do_big_coffee(_ekspres *ekspres);
_states do_espresso(_ekspres *ekspres);
_states do_cappucino(_ekspres *ekspres);
_states prepare_hot_milk(_ekspres *ekspres);
_states define_power(_ekspres *ekspres);
_states define_amount_of_water_for_small_coffee(_ekspres *ekspres);

state_fun* const state_table[NUM_STATES] =
{
	do_small_coffee, do_big_coffee, do_espresso, do_cappucino, prepare_hot_milk, define_power, define_amount_of_water_for_small_coffee
};

_states run_state(_states cur_state, _ekspres *ekspres)
{
	return state_table[cur_state](ekspres);
}
*/
void switch_off(_ekspres *ekspres)
{
	FILE *f;
	if ((f = fopen("data.txt", "w")) == NULL) {
		printf("File not found!\n");
		return;
	}

	fprintf(f, "%i", ekspres->amount_of_coffee);
	fprintf(f, "%i", ekspres->amount_of_water);
	fprintf(f, "%i", ekspres->amount_of_milk);
	fprintf(f, "%i", ekspres->power);
	fprintf(f, "%i", ekspres->base_amount_of_water);

	fclose(f);
	return;
}

void switch_on(_ekspres *ekspres)
{
	FILE *f;
	if ((f = fopen("data.txt", "w")) == NULL) {
		printf("File not found!\n");
		return;
	}

	fscanf(f, "%i", ekspres->amount_of_coffee);
	fscanf(f, "%i", ekspres->amount_of_water);
	fscanf(f, "%i", ekspres->amount_of_milk);
	fscanf(f, "%i", ekspres->power);
	fscanf(f, "%i", ekspres->base_amount_of_water);

	fclose(f);
	return;
}

int main()
{
	_ekspres *ekspres = malloc(sizeof(_ekspres));

	switch_on(ekspres);

	_getch();
	/*prinft("parametr %d", ekspres->amount_of_milk);
	prinft("%i", ekspres->amount_of_coffee);
	prinft("%i", ekspres->amount_of_water);*/

	//_states cur_state = STATE_INITIAL;		//oczekiwanie na komende
	return 0;
}