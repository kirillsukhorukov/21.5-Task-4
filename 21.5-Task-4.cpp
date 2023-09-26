#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstdlib>

//Объявление глобальных переменных
//Параметры поля
const unsigned int FIELD_SIZE = 20;
char field[FIELD_SIZE][FIELD_SIZE];

//Игровые параметры
const unsigned int COUNT_PLAYERS = 6;
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
void players_init(std::vector <character> &players)
{
    //Инициализация игрока
    std::cout << "Enter player name: ";
    std::getline(std::cin, players[0].name);
    players[0].health = input_int("Enter player's health level: ");
    players[0].armor = input_int("Enter player's armor level: ");
    players[0].damage = input_int("Enter player's damage level: ");
    players[0].place.X = random_num(0, FIELD_SIZE-1);
    players[0].place.Y = random_num(0, FIELD_SIZE-1);

    //Инициализация противников
    for (int i=1; i<COUNT_PLAYERS; i++)
    {
        players[i].name = "Enemy #" + std::to_string(i);
        players[i].health = random_num(MIN_HEALTH,MAX_HEALTH);
        players[i].armor = random_num(MIN_ARMOR,MAX_ARMOR);
        players[i].damage = random_num(MIN_DAMAGE,MAX_DAMAGE);
        players[i].place.X = random_num(0, FIELD_SIZE-1);
        players[i].place.Y = random_num(0, FIELD_SIZE-1);
        players[i].team = false;
    }
}

//Функция движения игрока
void movement (character &player, const int &direction)
{
    if (direction == 1) 
    {
        if (player.place.X-1 >=0) player.place.X--;
    }
    else if (direction == 2) 
    {
        if (player.place.X+1 <FIELD_SIZE) player.place.X++;
    }
    else if (direction == 3) 
    {
        if (player.place.Y-1 >=0) player.place.Y--;
    }
    else if (direction == 4) 
    {
        if (player.place.Y+1 <FIELD_SIZE) player.place.Y++;
    }
}

//Функция обновления игрового поля и его вывода на экран
void print_field (std::vector <character> &players)
{
    //обнулить поле
    for (int i=0; i<FIELD_SIZE; i++) 
    {
        for (int j=0; j<FIELD_SIZE; j++) field[i][j] = '.';
    }

    //Расстановка игроков
    for (int i=1; i<COUNT_PLAYERS; i++)
    {
        if (players[i].alive) field[players[i].place.Y][players[i].place.X] = 'E';
    }
    if (players[0].alive) field[players[0].place.Y][players[0].place.X] = 'P';

    //Вывод игрового поля
    std::cout << std::endl << "\t\t\t\t\t\t\t\t\t\t----- BATTLEFIELD -----" << std::endl << std::endl;
    
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

//Функция сохранения в файл
void save_data (std::vector <character> &players)
{

}

//Функция загрузки из файла
void load_data (std::vector <character> &players)
{

}

//Функция вывода информации о игроках
void players_info (std::vector <character> &players)
{
    std::cout << std::endl;
    std::cout << "NAME\t\tHEALTH\tARMOR\tDAMAGE\tX\tY\tALIVE" << std::endl;
    std::cout << players[0].name << "\t\t" << players[0].health << "\t" << players[0].armor << "\t" 
        << players[0].damage << "\t" << players[0].place.X+1 << "\t" << players[0].place.Y+1 << "\t" 
        << std::boolalpha << players[0].alive << std::endl;
    
    for (int i=1; i<COUNT_PLAYERS; i++)
    {
        std::cout << players[i].name << "\t" << players[i].health << "\t" << players[i].armor << "\t" 
            << players[i].damage << "\t" << players[i].place.X+1 << "\t" << players[i].place.Y+1 << "\t" 
            << std::boolalpha << players[i].alive << std::endl;
    }
    std::cout << std::endl;
}

//Функция вывода информации о командах
void print_command_info()
{
    std::cout << std::endl;
    std::cout << "Game control:" << std::endl << std::endl;
    std::cout << "'L' - step left;" << std::endl; 
    std::cout << "'R' - step right;" << std::endl; 
    std::cout << "'U' - step up;" << std::endl; 
    std::cout << "'L' - step down;" << std::endl;
    std::cout << "'save' - save to file;" << std::endl;
    std::cout << "'load' - load from file;" << std::endl;
    std::cout << "'info' - view players information;" << std::endl;
    std::cout << "'help' - view commands;" << std::endl;
    std::cout << "'quit' - terminate program execution." << std::endl << std::endl;
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

//Функция проверки поединка
bool check_fight(std::vector <character> &players, const bool &team)
{
    for (int i=1; i<COUNT_PLAYERS; i++)
    {
        //Удар наносит игорок
        if (players[0].place.X==players[i].place.X & players[0].place.Y==players[i].place.Y & team) 
        {
            take_damage(players[i].health, players[i].armor, players[0].damage, players[i].alive);
            return true;
        }
        //Удар наносит противник
        if (players[0].place.X==players[i].place.X & players[0].place.Y==players[i].place.Y & !team) 
        {
            take_damage(players[0].health, players[0].armor, players[i].damage, players[0].alive);
            return true;
        }
    }
    return false;
}

//Функция проверки победы одной из сторон
bool victory (std::vector <character> &players)
{
    //Поражение игрока
    if (!players[0].alive) 
    {
        std::cout << std::endl << players[0].name << " lost!" << std::endl;
        players_info(players);
        return true;
    }

    //Поражение противников
    bool enemy_death = players[1].alive;
    for (int i=2; i<COUNT_PLAYERS; i++) enemy_death += players[i].alive; 
    if (!enemy_death) 
    {
        std::cout << std::endl << players[0].name << " won!" << std::endl;
        players_info(players);
        return true;
    }
    
    return false;
}

//Функция одного цикла в игре
void move (std::vector <character> &players, bool &quit)
{
    //Переменная направления движения (1 - влево, 2 - вправо, 3 - вверх, 4 - вниз)
    int direction = 1;

    //Ввод команды игроком
    bool error = false;
    std::string command;
    do
    {
        std::cout << std::endl << "Enter the command:";
        error = false;
        std::getline(std::cin,command);
        if (command == "L") direction = 1;
        else if (command == "R") direction = 2;
        else if (command == "U") direction = 3;
        else if (command == "D") direction = 4;
        else if (command == "save") 
        {
            save_data(players);
            error = true;
        }
        else if (command == "load") 
        {
            load_data(players);
            error = true;
        }
        else if (command == "info") 
        {
            players_info(players);
            error = true;
        }
        else if (command == "help") 
        {
            print_command_info();
            error = true;
        }
        else if (command == "quit") 
        {
            std::cout << "--- Program completed ---" << std::endl;
            quit = true;
            return;
        }
        else 
        {
            std::cerr <<"Error! Input 'help' for view commands. Repeat input." << std::endl;
            error = true;
        }
    } while (error);
    
    //Цикл ходов всех участников
    //Флаг, что бой завершен
    bool end_fight=false;
    for (int i=0; i<COUNT_PLAYERS; i++)
    {
        //Движение игрока
        movement(players[i], direction);
        
        if (!end_fight)
        {
            //Проверка поединка
            if (check_fight(players, players[i].team))
            {
                //Если поединок состоялся, то проверка победы одной из сторон
                if (victory(players))
                {
                    quit = true;
                    return;
                }
                end_fight = true;
            }
        }
        
        
        //Генерация направления хода для противников
        direction = random_num(1,4);
    }
    
    //Отображение игрового поля
    print_field(players);
    players_info(players);
}

int main()
{
    //Задание начального зерна случайных чисел
    std::srand(std::time(nullptr));

    //Инициализация массива игроков
    std::vector <character> players(COUNT_PLAYERS);

    //Начальный экран
    std::cout << "------ SKILLBOX RPG v1.1 ------" << std::endl << std::endl;
    std::cout << "Enter the command:" << std::endl << std::endl;
    std::cout << "'new' - start new game;" << std::endl;
    std::cout << "'load' - load from file;" << std::endl;
    std::cout << "'quit' - terminate program execution." << std::endl << std::endl;
    
    //Загрузка данных игроков
    bool error = false;
    std::string command;
    do
    {
        error = false;
        std::getline(std::cin,command);
        if (command == "new") players_init(players);
        else if (command == "load") load_data(players);
        else if (command == "quit") 
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
    
    //Вывод игрового поля информации игроков и информации о командах управления
    print_field(players);
    players_info(players);
    print_command_info();
    
    //Игровой процесс
    std::cout << std::endl << "\t\t\t\t\t\t\t\t----- Beginning of the game -----" << std::endl << std::endl;
    bool quit = false;  //Флаг окончания игры
    while (!quit)
    {
        move(players,quit);
    }

    return 1;
}