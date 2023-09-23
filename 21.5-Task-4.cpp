#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstdlib>

//Объявление глообальных перемненых
//Параметры поля
const unsigned int FIELD_SIZE = 20;
char field[FIELD_SIZE][FIELD_SIZE];

//Игровые параметры
const unsigned int COUNT_ENEMY = 5;
const unsigned int MIN_HEALTH = 50;
const unsigned int MAX_HEALTH = 150;
const unsigned int MIN_ARMOR = 0;
const unsigned int MAX_ARMOR = 50;
const unsigned int MIN_DAMAGE = 15;
const unsigned int MAX_DAMAGE = 30;

struct coordinate
{
    int X = 0;
    int Y = 0;
};

struct character
{
    std::string name = "unknown";
    int health = 0;
    int armor = 0;
    int damage = 0;
    coordinate place;
    bool alive = true; //Флаг живой персонаж или нет
    bool team = true;  //Флаг принадлежности к команде (1 - игрок; 0 - команда противника)
};

//Функция инициализации поля
void field_init()
{
    for (int i=0; i<FIELD_SIZE; i++) 
    {
        for (int j=0; j<FIELD_SIZE; j++) field[i][j] = '.';
    }
    return;
}

//Функция прорисовки поля
void field_print()
{
    std::cout << "\t";
    
    for (int x=0; x<FIELD_SIZE; x++) std::cout << x+1 << "\t"; 
    std::cout << std::endl;
    for (int i=0; i<FIELD_SIZE; i++) 
    {
        std::cout << i+1 << "\t";
        for (int j=0; j<FIELD_SIZE; j++) std::cout << field[i][j] << "\t";
        std::cout << std::endl;
    }
    return;
}

//Функция генерации случайного целого числа
int random_num(const int &min, const int &max)
{
    return (std::rand()%(max-min))+min;
}

//Функция проверки строки с целым числом
bool check_int(std::string &num)
{
    //Проверка длины строки с числом
    if (num.length() > 9)
    {
        std::cerr << std::endl << "Error! The number is not a positive integer!" << std::endl;
        return false;
    }
    else 
    {
        //Проверка что все символы являются числами
        for (int i = 0; i < num.length(); i++)
        {
            if (!isdigit(num[i]))
                {
                    std::cerr << std::endl << "Error! The number is not a positive integer!" << std::endl;
                    return false;
                }
        }
    }
    return true;
}

//Функция ввода целого положительного числа 
int input_int (const std::string &str)
{
    std::string temp;
    
    do
    {
        std::cout << str;
        std::getline(std::cin, temp);
    } while (!check_int(temp));

    int num = stoi (temp);
    return num;
}

//Функция инициализации игроков
void players_init(character &player, std::vector <character> &enemies)
{
    //Инициализация игрока
    std::cout << "Enter player name: ";
    std::getline(std::cin, player.name);
    player.health = input_int("Enter player's health level: ");
    player.armor = input_int("Enter player's armor level: ");
    player.damage = input_int("Enter player's damage level: ");
    player.place.X = random_num(0, FIELD_SIZE-1);
    player.place.Y = random_num(0, FIELD_SIZE-1);

    //Инициализация противников
    for (int i=0; i<COUNT_ENEMY; i++)
    {
        enemies[i].name = "Enemy #" + std::to_string(i+1);
        enemies[i].health = random_num(MIN_HEALTH,MAX_HEALTH);
        enemies[i].armor = random_num(MIN_ARMOR,MAX_ARMOR);
        enemies[i].damage = random_num(MIN_DAMAGE,MAX_DAMAGE);
        enemies[i].place.X = random_num(0, FIELD_SIZE-1);
        enemies[i].place.Y = random_num(0, FIELD_SIZE-1);
        enemies[i].team = false;
    }
}

//Функция вывода информации о игроках
void players_info (character &player, std::vector <character> &enemies)
{
    std::cout << std::endl;
    std::cout << "NAME\t\tHEALTH\tARMOR\tDAMAGE\tX\tY\tALIVE" << std::endl;
    std::cout << player.name << "\t\t" << player.health << "\t" << player.armor << "\t" 
        << player.damage << "\t" << player.place.X << "\t" << player.place.X << "\t" 
        << std::boolalpha << player.alive << std::endl;
    
    for (int i=0; i<COUNT_ENEMY; i++)
    {
        std::cout << enemies[i].name << "\t" << enemies[i].health << "\t" << enemies[i].armor << "\t" 
            << enemies[i].damage << "\t" << enemies[i].place.X << "\t" << enemies[i].place.X << "\t" 
            << std::boolalpha << enemies[i].alive << std::endl;
    }
}

//Функция сохранения в файл
void save_data (character &player, std::vector <character> &enemies)
{

}

//Функция загрузки из файла
void load_data (character &player, std::vector <character> &enemies)
{

}

//Функция нанесения урона
void take_damage(int &health, int &armor, const int &damage, bool &alive)
{
    armor -= damage;
    if (armor <0)
    {
        health += armor;
        armor = 0;
    }
    if (health < 0) alive = false;
}

int main()
{
    //Задание начального зерна случайных чисел
    std::srand(std::time(nullptr));

    //Инициализация пустого поля
    field_init();

    //Инициализация игрока
    character player;

    //Инициализация массива противников
    std::vector <character> enemies(COUNT_ENEMY);

    //Начальный экран
    std::cout << "------ SKILLBOX RPG ------" << std::endl << std::endl;
    std::cout << "Enter the command:" << std::endl << std::endl;
    std::cout << "'new' - start new game;" << std::endl;
    std::cout << "'load' - load from file;" << std::endl;
    std::cout << "'quit' - terminate program execution." << std::endl << std::endl;
    
    //Загрузка данных игроков
    bool error = false;
    std::string str;
    do
    {
        error = false;
        std::getline(std::cin,str);
        if (str == "new") players_init(player,enemies);
        else if (str == "load") load_data(player,enemies);
        else if (str == "quit") 
        {
            std::cout << "--- Program completed ---" << std::endl;
            return 1;
        }
        else 
        {
            std::cerr <<"Error! Repeat input." << std::endl;
            error = true;
        }
    } while (error);

    players_info(player,enemies);

    return 1;
}