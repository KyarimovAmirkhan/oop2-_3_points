#include "GameController.h"
#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        int fieldWidth, fieldHeight;
        std::cout << "Введите размеры поля (ширина высота): ";
        if (!(std::cin >> fieldWidth >> fieldHeight))
        {
            std::cout << "Ошибка ввода! Введите числа.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            return 1;
        }
        if (fieldWidth <= 0 || fieldHeight <= 0)
        {
            std::cout << "Размеры поля должны быть положительными числами!\n";
            return 1;
        }
        GameController game(fieldWidth, fieldHeight);
        game.startGame();
    }
    catch (const std::exception& e)
    {
        std::cout << "Критическая ошибка: " << e.what() << "\n";
        return 1;
    }
    catch (...)
    {
        std::cout << "Неизвестная критическая ошибка!\n";
        return 1;
    }
    return 0;
}