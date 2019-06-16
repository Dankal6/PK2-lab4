#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
//#include <cctype>

//jednostki moga byc dowolne, najlepiej inty, ekspres moze sie uzupelniac sam (smart ekspres), lub na polecenie uzytkownika, stany zczytywane scanf
//sprawdzanie poprawnosci (np moc kawy 1-10, jak ktos wpisze a nie ma sie wywalac)

typedef enum {
	STATE_INITIAL, STATE_WAIT, STATE_REFILL_WATER, STATE_REFILL_MILK, STATE_REFILL_COFREE,
	STATE_SMALL_COFFEE, STATE_BIG_COFFEE, STATE_ESPRESSO, STATE_CAPPUCINO, STATE_HOT_MILK,
	STATE_DEF_POWER, STATE_AMOUNT_OF_WATER, SWITCH_OFF, NUM_STATES
} _states;

typedef enum { WATER, MILK, COFFEE };

typedef struct {
	float amount_of_water;
	float amount_of_milk;
	float amount_of_coffee;
	int power;
	int base_amount_of_water;
}_ekspres;

typedef _states state_fun(_ekspres *ekspres);

_states init(_ekspres *ekspres);
_states wait_for_command(_ekspres *ekspres);
_states refill_water(_ekspres *ekspres);
_states refill_milk(_ekspres *ekspres);
_states refill_coffee(_ekspres *ekspres);
_states do_small_coffee(_ekspres *ekspres);
_states do_big_coffee(_ekspres *ekspres);
_states do_espresso(_ekspres *ekspres);
_states do_cappucino(_ekspres *ekspres);
_states prepare_hot_milk(_ekspres *ekspres);
_states define_power(_ekspres *ekspres);
_states define_amount_of_water_for_small_coffee(_ekspres *ekspres);
_states switch_off(_ekspres *ekspres);

state_fun* const state_table[NUM_STATES] =
{
	init,wait_for_command,refill_water,refill_milk,refill_coffee,do_small_coffee, do_big_coffee, do_espresso, do_cappucino, prepare_hot_milk, define_power, define_amount_of_water_for_small_coffee,switch_off
};

_states run_state(_states cur_state, _ekspres *ekspres)
{
	return state_table[cur_state](ekspres);
}

_states switch_off(_ekspres *ekspres)
{
	FILE *f;
	if ((f = fopen("data.txt", "w")) == NULL) {
		printf("File not found!\n");
		return;
	}

	fprintf(f, "%f\n", ekspres->amount_of_coffee);
	fprintf(f, "%f\n", ekspres->amount_of_water);
	fprintf(f, "%f\n", ekspres->amount_of_milk);
	fprintf(f, "%i\n", ekspres->power);
	fprintf(f, "%i\n", ekspres->base_amount_of_water);

	fclose(f);
	exit(0);
}


void switch_on(_ekspres *ekspres)
{
	FILE *f;
	if ((f = fopen("data.txt", "r")) == NULL) {
		printf("File not found!\n");
		//wartosci poczatkowe, jezeli nie ma pliku "data" - symulacja tego, jakby ekspres byl swiezo z pudelka wyciagniety
		ekspres->amount_of_coffee = 0;
		ekspres->amount_of_water = 0;
		ekspres->amount_of_milk = 0;
		ekspres->power = 5;
		ekspres->base_amount_of_water = 100;
		return;
	}

	fscanf(f, "%f", &ekspres->amount_of_coffee);
	fscanf(f, "%f", &ekspres->amount_of_water);
	fscanf(f, "%f", &ekspres->amount_of_milk);
	fscanf(f, "%i", &ekspres->power);
	fscanf(f, "%i", &ekspres->base_amount_of_water);

	fclose(f);
	return;
}

void show_ekspres(_ekspres ekspres)
{
	system("cls");
	printf("Ilosc wody: %f\n", ekspres.amount_of_water);
	printf("Ilosc mleka: %f\n", ekspres.amount_of_milk);
	printf("Ilosc kawy: %f\n", ekspres.amount_of_coffee);
	printf("'Moc' kawy: %i\n", ekspres.power);
	printf("Bazowa wielkosc kawy: %i\n", ekspres.base_amount_of_water);
}

//uzupelnianie jest na polecenie uzytkownika
void fill(_ekspres *ekspres, int what)
{
	float *resource = NULL;
	if (what == WATER)
	{
		resource = &ekspres->amount_of_water;
	}
	else if (what == MILK)
	{
		resource = &ekspres->amount_of_milk;
	}
	else if (what == COFFEE)
	{
		resource = &ekspres->amount_of_coffee;
	}
	float time = (int)((1000 - (*resource)) / 50);
	for (int i = 0; i < time;)
	{
		(*resource) += 50;
		show_ekspres(*ekspres);
		printf("Nacisnij cokolwiek, by anulowac.\n");
		Sleep(1000);
		if (kbhit())
		{
			getch();
			return;
		}
		time--;
	}
	if (time == 0 && (1000 - (*resource)) != 0)
	{
		show_ekspres(*ekspres);
		printf("Nacisnij cokolwiek, by anulowac.\n");
		Sleep(500);
		if (kbhit())
		{
			getch();
			return;
		}
		(*resource) = 1000;
	}
}

int check_for_resources(_ekspres ekspres, float _water, float _milk, float _coffee)
{
	float difference = 0;
	if (ekspres.amount_of_water < _water)
	{
		difference = _water - ekspres.amount_of_water;
		printf("Brakuje %f wody!\n", difference);
	}
	if (ekspres.amount_of_milk < _milk)
	{
		difference = _milk - ekspres.amount_of_milk;
		printf("Brakuje %f mleka!\n", difference);
	}
	if (ekspres.amount_of_coffee < _coffee)
	{
		difference = _coffee - ekspres.amount_of_coffee;
		printf("Brakuje %f kawy!\n", difference);
	}
	if (difference != 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int get_number_from_user(int a, int b)
{
	int num;
	fseek(stdin, 0, SEEK_END);
	scanf("%i", &num);
	//int x = isdigit(num);
	if (num >= a && num <= b)
	{
		return num;
	}
	else
	{
		printf("Wartosc nieprawidlowa!\n");
		_getch();
		return -1;
	}
}

int main()
{
	_ekspres *ekspres = malloc(sizeof(_ekspres));
	_states cur_state = STATE_INITIAL;
	cur_state = run_state(cur_state, ekspres);
	wait_for_command(ekspres);
	return 0;
}

_states init(_ekspres * ekspres)
{
	switch_on(ekspres);
	return wait_for_command(ekspres);
}

_states wait_for_command(_ekspres * ekspres)
{
	int choice = -1;
	_states cur_state;
	fseek(stdin, 0, SEEK_END);

	while (choice == -1)
	{
		show_ekspres(*ekspres);
		printf("\n1. Uzupelnij wode.\n");
		printf("2. Uzupelnij mleko.\n");
		printf("3. Uzupelnij kawe.\n");
		printf("4. Przygotuj mala kawe.\n");
		printf("5. Przygotuj duza kawe.\n");
		printf("6. Przygotuj espresso.\n");
		printf("7. Przygotuj cappucino.\n");
		printf("8. Przygotuj gorace mleko.\n");
		printf("9. Zdefiniuj 'moc' kawy.\n");
		printf("10. Zdefiniuj bazowa wielkosc kawy.\n");
		printf("0. Wylacz ekspres.\n");
		choice = get_number_from_user(0, 10);
	}

	switch (choice)
	{
	case 1:
		cur_state = STATE_REFILL_WATER;
		break;
	case 2:
		cur_state = STATE_REFILL_MILK;
		break;
	case 3:
		cur_state = STATE_REFILL_COFREE;
		break;
	case 4:
		cur_state = STATE_SMALL_COFFEE;
		break;
	case 5:
		cur_state = STATE_BIG_COFFEE;
		break;
	case 6:
		cur_state = STATE_ESPRESSO;
		break;
	case 7:
		cur_state = STATE_CAPPUCINO;
		break;
	case 8:
		cur_state = STATE_HOT_MILK;
		break;
	case 9:
		cur_state = STATE_DEF_POWER;
		break;
	case 10:
		cur_state = STATE_AMOUNT_OF_WATER;
		break;
	case 0:
		cur_state = SWITCH_OFF;
	}
	return run_state(cur_state, ekspres);
}

_states refill_water(_ekspres * ekspres)
{
	fill(ekspres, WATER);
	return wait_for_command(ekspres);
}

_states refill_milk(_ekspres * ekspres)
{
	fill(ekspres, MILK);
	return wait_for_command(ekspres);
}

_states refill_coffee(_ekspres * ekspres)
{
	fill(ekspres, COFFEE);
	return wait_for_command(ekspres);
}

_states do_small_coffee(_ekspres * ekspres)
{
	int can = check_for_resources(*ekspres, ekspres->base_amount_of_water
		, (((10 - ekspres->power) * ekspres->base_amount_of_water) / 10)
		, ((ekspres->power * ekspres->base_amount_of_water) / 10));
	if (can == 1)
	{
		system("cls");
		printf("Przygotowuje mala kawe...\n");
		ekspres->amount_of_water -= ekspres->base_amount_of_water;
		ekspres->amount_of_coffee -= (ekspres->power * ekspres->base_amount_of_water) / 10;
		ekspres->amount_of_milk -= ((10 - ekspres->power) * ekspres->base_amount_of_water) / 10;
		Sleep(1000);
		printf("Mala kawa gotowa!");
	}
	Sleep(1000);
	return wait_for_command(ekspres);
}

_states do_big_coffee(_ekspres * ekspres)
{
	int can = check_for_resources(*ekspres, ekspres->base_amount_of_water * 2
		, (((10 - ekspres->power) * ekspres->base_amount_of_water) / 5)
		, ((ekspres->power * ekspres->base_amount_of_water) / 5));
	if (can == 1)
	{
		system("cls");
		printf("Przygotowuje duza kawe...\n");
		ekspres->amount_of_water -= ekspres->base_amount_of_water * 2;
		ekspres->amount_of_coffee -= (ekspres->power * ekspres->base_amount_of_water) / 5;
		ekspres->amount_of_milk -= ((10 - ekspres->power) * ekspres->base_amount_of_water) / 5;
		Sleep(1000);
		printf("Duza kawa gotowa!");
	}
	Sleep(1000);
	return wait_for_command(ekspres);
}

_states do_espresso(_ekspres * ekspres)		//25 ml kawy, bez mleka
{
	int can = check_for_resources(*ekspres, ekspres->base_amount_of_water / 2
		, 0
		, ekspres->base_amount_of_water / 2);
	if (can == 1)
	{
		system("cls");
		printf("Przygotowuje espresso...\n");
		ekspres->amount_of_water -= ekspres->base_amount_of_water / 2;
		ekspres->amount_of_coffee -= ekspres->base_amount_of_water / 2;
		Sleep(1000);
		printf("Espresso gotowe!");
	}
	Sleep(1000);
	return wait_for_command(ekspres);
}

_states do_cappucino(_ekspres * ekspres)	//25 ml kawy, 125 mleka
{
	int can = check_for_resources(*ekspres, ekspres->base_amount_of_water*1.5
		, ekspres->base_amount_of_water *1.25
		, ekspres->base_amount_of_water / 4);
	if (can == 1)
	{
		system("cls");
		printf("Przygotowuje cappucino ...\n");
		ekspres->amount_of_water -= ekspres->base_amount_of_water * 1.5;
		ekspres->amount_of_coffee -= ekspres->base_amount_of_water / 4;
		ekspres->amount_of_milk -= ekspres->base_amount_of_water *1.25;
		Sleep(1000);
		printf("Cappucino gotowe!");
	}
	Sleep(1000);
	return wait_for_command(ekspres);
}

_states prepare_hot_milk(_ekspres * ekspres)
{
	int can = check_for_resources(*ekspres, 0, (ekspres->base_amount_of_water *0.5) , 0);
	if (can == 1)
	{
		system("cls");
		printf("Przygotowuje gorace mleko ...\n");
		ekspres->amount_of_milk -= ekspres->base_amount_of_water *0.5;
		Sleep(1000);
		printf("Gorace mleko gotowe!");
	}
	Sleep(1000);
	return wait_for_command(ekspres);
}

_states define_power(_ekspres * ekspres)
{
	int power = -1;
	while (power == -1)
	{
		printf("Moc (1-10): ");
		power = get_number_from_user(1, 10);
	}
	ekspres->power = power;
	return wait_for_command(ekspres);
}

_states define_amount_of_water_for_small_coffee(_ekspres * ekspres)
{
	int amount = -1;
	while (amount == -1)
	{
		printf("Bazowa wielkosc kawy (1-200): ");
		amount = get_number_from_user(1, 200);
	}
	ekspres->base_amount_of_water = amount;
	return wait_for_command(ekspres);
}

