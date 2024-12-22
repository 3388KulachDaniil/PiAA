#pragma once

#include <string>
#include <vector>

using namespace std;

class Annotation{
    public:
        string DEFAULT_CONTROL = "Управление будет установленно по умолчанию\n";
        string SHIPS_INVALID = "Некорректный ввод поля с кораблями. Завершение работы\n";
        string PLAYER_FIELD = "Ваше поле:\n";
        string ENEMY_FIELD = "Поле противника:\n";
        string DRAW = "Ничья! Попробуйте еще\n";
        string WIN = "Победа! Новый раунд с новым противником (ваше состояние сохраняется)\n";
        string LOSE = "Поражение! Попробуйте еще\n";
        string USE_SKILL_Q = "Хотите использовать навык? ";
        string SET_COORDS_SKILL_Q = "Хотите ввести координаты навыка? ";
        string INPUT_SKILL_COORDS = "Введите координаты навыка: ";
        string INPUT_ATTACK_COORDS = "Введите координаты удара: ";
        string SKILL_ANS = "Результат навыка (bool): ";
        string INPUT_FILENAME = "Введите имя файла: ";
        string INPUT_SHIPS = "Введите количество кораблей длины 1, 2, 3, 4: ";
        string FIELD_INPUT = "Введите размер поля в ширину и высоту: ";
        string SKIP_SHIP = "Не удалось установить корабль, пропуск\n";
        vector<string> HELP_LINES = {
            "Справка:", "\tЗагрузить игру", "\tСохранить игру", "\tАтаковать", "\tОчередь навыков", "\tСправка"
        };
};