#include "playing_field.h"
#include "skills/i_attack_creator.h"
#include "skills/simple_attack_creator.h"


ostream& operator<<(ostream& stream, const PlayingField& playing_field){
    for (size_t y = 0; y < playing_field.height; ++y){
        for (size_t x = 0; x < playing_field.width; ++x){
            cout<<playing_field.field[y][x]<<" ";
        }
        cout<<endl;
    }
    return stream;
}

PlayingField::PlayingField(size_t width, size_t height){
    this->width = width;
    this->height = height;
    this->attack_type = make_shared<SimpleAttack>();
    this->field = new Cell*[height];
    for (size_t i = 0; i < height; ++i)
        this->field[i] = new Cell[width]();
}

PlayingField::PlayingField(const PlayingField& playing_field){
    this->width = playing_field.width;
    this->height = playing_field.height;
    this->attack_type = playing_field.attack_type;

    this->field = new Cell*[this->height];
    for (size_t i = 0; i < this->height; ++i){
        this->field[i] = new Cell[this->width];
        for (size_t j = 0; j < this->width; ++j)
            this->field[i][j] = playing_field.field[i][j];
    }
}

PlayingField& PlayingField::operator=(const PlayingField& playing_field){
    if (this != &playing_field){
        if (this->field){
            for (size_t i = 0; i < height; ++i)
                delete[] field[i];
            delete[] field;
        }

        this->width = playing_field.width;
        this->height = playing_field.height;
        this->attack_type = playing_field.attack_type;

        this->field = new Cell*[this->height];
        for (size_t i = 0; i < this->height; ++i){
            this->field[i] = new Cell[this->width];
            for (size_t j = 0; j < this->width; ++j)
                this->field[i][j] = playing_field.field[i][j];
        }
    }
    return *this;
}


PlayingField::PlayingField(PlayingField&& playing_field){
    this->width = playing_field.width;
    this->height = playing_field.height;
    this->field = playing_field.field;
    this->attack_type = move(playing_field.attack_type);

    playing_field.width = 0;
    playing_field.height = 0;
    playing_field.field = nullptr;
    playing_field.attack_type = nullptr;
}

PlayingField& PlayingField::operator=(PlayingField&& playing_field){
    if (this != &playing_field){
        if (this->field){
            for (size_t i = 0; i < height; ++i)
                delete[] field[i];
            delete[] field;
        }

        this->width = playing_field.width;
        this->height = playing_field.height;
        this->field = playing_field.field;
        this->attack_type = move(playing_field.attack_type);

        playing_field.width = 0;
        playing_field.height = 0;
        playing_field.field = nullptr;
        playing_field.attack_type = nullptr; 
    }
    return *this;
}


PlayingField::~PlayingField(){
    if (this->field){
        for (size_t i = 0; i < this->height; ++i)
            delete [] this->field[i];
        delete [] this->field;
    }
}

bool PlayingField::is_cell_alone(long int x, long int y){
    long int w = static_cast<long int>(this->width);
    long int h = static_cast<long int>(this->height);
    if (x >= w || x < 0 || y >= h || y < 0)
        return false;
    for (long int i = y - 1; i <= y + 1; ++i){
        for (long int j = x - 1; j <= x + 1; ++j){
            if (((0 <= j && j < w) && (0 <= i && i < h)) && !(this->field[i][j].isEmpty()))
                return false;
        }
    }

    return true;
}

void PlayingField::addShip(size_t x, size_t y, Ship& ship, Ship::Orientation orientation){
    if (x >= this->width || y >= this->height)
        throw OutOfFieldException();

    if (orientation == Ship::Orientation::VERTICAL){
        for (size_t i = y; i < y + ship.size(); ++i)
            if (!(this->is_cell_alone(x, i)))
                throw CoordinatesAddShipException();

        for (size_t i = y; i < y + ship.size(); ++i)
            this->field[i][x].setSegment(ship[i - y]);
    }
    else{
        for (size_t i = x; i < x + ship.size(); ++i)
            if (!(this->is_cell_alone(i, y)))
                throw CoordinatesAddShipException();

        for (size_t i = x; i < x + ship.size(); ++i)
            this->field[y][i].setSegment(ship[i - x]);
    }

    ship.setOrientation(orientation);
}

void PlayingField::base_attack(size_t x, size_t y, unsigned short int damage){
    if (x >= this->width || y >= this->height)
        throw OutOfFieldAttackException();
    
    if (!(this->field[y][x].isEmpty())){
        this->field[y][x].getSegment().changeStateDelta(-damage);
        this->field[y][x].setState(Cell::ViewState::SHIP);
        return;
    }
    this->field[y][x].setState(Cell::ViewState::EMPTY);
}

void PlayingField::printFull(){
    for (size_t y = 0; y < this->height; ++y){
        for (size_t x = 0; x < this->width; ++x){
            if (this->field[y][x].isEmpty()){
                cout<<"□"<<" ";
            }
            else{
                switch (this->field[y][x].getSegment().getState()){
                    case Segment::State::UNDAMAGED:
                        cout<<2<<" ";
                        break;
                    case Segment::State::DAMAGED:
                        cout<<1<<" ";
                        break;
                    default:
                        cout<<"x"<<" ";
                }
            }
        }
        cout<<endl;
    }
}

Cell& PlayingField::getCell(size_t x, size_t y){
    if (x >= this->width || y >= this->height)
        throw OutOfFieldException();
    return this->field[y][x];
}

size_t PlayingField::getWidth(){
    return this->width;
}

size_t PlayingField::getHeight(){
    return this->height;
}

void PlayingField::setDefaultAttack(){
    this->attack_type = SimpleAttackCreator().create();
}

void PlayingField::setAttackType(shared_ptr<IAttack> attack_type){
    this->attack_type = attack_type;
}

void PlayingField::attack(size_t x, size_t y, unsigned short int damage){
    this->attack_type->attack(*this, x, y, damage);
}

bool PlayingField::isAllShipsDead(){
    for (size_t i = 0; i < this->height; ++i){
        for (size_t j = 0; j < this->width; ++j){
            if (!(this->field[i][j].isEmpty()) && this->field[i][j].getSegment().getState() != Segment::State::DESTROYED)
                return false;
        }
    }
    return true;
}