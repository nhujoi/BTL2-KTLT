#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// class MapElement
MapElement::MapElement(ElementType in_type)
{
    this->type = in_type;
}

MapElement::~MapElement() {}

ElementType MapElement::getType() const
{
    return type;
}

// clas Position
Position::Position(int r, int c) : r(r), c(c) {}

Position::Position(const string & str_pos)
{
    string sr = str_pos.substr(1, str_pos.length() - 2);
    int idx = sr.find(",");
    string str_row = sr.substr(0, idx);
    string str_col = sr.substr(idx + 1, sr.length() - idx - 1);
    r = stoi(str_row);
    c = stoi(str_col);
}

int Position::getRow() const {
    return r;
}
int Position::getCol() const {
    return c;
}
void Position::setRow(int r) {
    this->r = r;
}
void Position::setCol(int c) {
    this->c = c;
}

string Position::str() const {
    return "(" + to_string(r) + "," + to_string(c) + ")";
}

bool Position::isEqual(int in_r, int in_c) const {
    if (r == in_r && c == in_c)
        return true;
    return false;
}

//clas Map

Map::Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls)
{
    this->num_rows = num_rows;
    this->num_cols = num_cols;
    map = new MapElement **[num_rows];
    for (int r = 0; r < num_rows; r++) {
        map[r] = new MapElement *[num_cols];    
    }

    for (int r = 0; r < num_rows; r++)
    {
        for (int c = 0; c < num_cols; c++) {
            map[r][c] = new Path();
        }
    }
    for (int i = 0; i < num_walls; i++) {
        int r = array_walls[i].getRow();
        int c = array_walls[i].getCol();
        map[r][c] = new Wall();
    }

    for (int i = 0; i < num_fake_walls; ++i)
    {
        int r = array_fake_walls[i].getRow();
        int c = array_fake_walls[i].getCol();
        int required_num = (r * 257 + c * 139 + 89) % 900 + 1;
        map[r][c] = new FakeWall(required_num);
    }
    
}

Map::~Map()
{
    for (int i = 0; i < num_rows; ++i) {
        delete[] map[i];
    }
    delete[] map;
}


bool Map::isValid(const Position &pos, MovingObject *mv_obj) const {
    int r = pos.getRow();
    int co = pos.getCol();

    if (r < 0 || r >= num_rows || co < 0 || co >= num_cols) {
        return false;
    }

    MapElement* e = map[r][co];
    if(e->getType() == FAKE_WALL) {
        bool isWatson = mv_obj->str().find("Watson") != string::npos;
        if(isWatson) {
            Watson* waston = dynamic_cast<Watson*>(mv_obj);
            int req_exp = dynamic_cast<FakeWall*>(e)->getReqExp();
            if(waston->exp <= req_exp) return false;
        }
    }
    else if (e->getType() == WALL) return false;


    return true;



}

// class MovingObject





const Position Position::npos = Position(-1, -1);

MovingObject::MovingObject(int index, const Position pos, Map *map, const string & name)
: index(index), pos(pos), map(map), name(name) {}

MovingObject::~MovingObject() {}

// class Character 





Character::Character(int index, const Position pos, Map * map, const string & name, int init_hp, int init_exp)
: MovingObject(index, pos, map, name), nums_move(0) {
    hp = init_hp;
    exp = init_exp;

    if(hp < 0) hp = 0;
    else {
        if(hp > 500) hp = 500;
        
    }
    if(exp < 0) exp = 0;
    else {
        if(exp > 900) exp = 900;
    }
};

Character::~Character() {}







void Character::setHp(int in_hp) {
    if(in_hp < 0) hp = 0;
    else {
        if(in_hp > 500) hp = 500;
        else {
            hp = in_hp;
        }
    }
    
}









void Character::setExp(int in_exp) {
    if(in_exp < 0) exp = 0;
    else {
        if(in_exp > 900) exp = 900;
        else {
            exp = in_exp;
        }
    }
}
string Character::getName() const {
    return name;
}
int Character::getDistanceToPosition(Position position) const {
    int a = abs(pos.getRow() - position.getRow());
    int b = abs(pos.getCol() - position.getCol());
    return a + b;
}
















// class Sherlock

Sherlock::Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) 
: Character(index, init_pos, map, "Sherlock", init_hp, init_exp) {
    this->moving_rule = moving_rule;
}

Position Sherlock::getNextPosition() {
    int length = moving_rule.size();
    if(length == 0) {
        return Position::npos;
    }
    int current = nums_move % length;
    int r = pos.getRow();
    int c = pos.getCol();

    if(moving_rule[current] == 'L') c--;
    else if(moving_rule[current] == 'R') c++;
    else if(moving_rule[current] == 'U') r--;
    else if(moving_rule[current] == 'D') r++;
    
    Position next_pos(r, c);
    if(map->isValid(next_pos, this) && hp != 0 && exp != 0) {
        return next_pos;
    } 
    return Position::npos;


}

void Sherlock::move() {
    Position next_pos = getNextPosition();
    if(!next_pos.isEqual(Position::npos.getRow(), Position::npos.getCol())) {
        pos = next_pos;
    }
    nums_move++;
}

string Sherlock::str() const {
    return "Sherlock[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}



// class Watson

Watson::Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp) 
: Character(index, init_pos, map, "Watson", init_hp, init_exp) {
    this->moving_rule = moving_rule;
 }







Position Watson::getNextPosition() {
    int le = moving_rule.size();
    if(le == 0) {
        return Position::npos;
    }
    int current = nums_move % le;
    int r = pos.getRow();
    int c = pos.getCol();

    if(moving_rule[current] == 'L') c--;
    else if(moving_rule[current] == 'R') c++;
    else if(moving_rule[current] == 'U') r--;
    else if(moving_rule[current] == 'D') r++;
    
    Position next_pos(r, c);
    if(map->isValid(next_pos, this) && hp != 0 && exp != 0) {
        return next_pos;
    } 
    return Position::npos;
}

void Watson::move() {
    Position next_pos = getNextPosition();
    
    if(!next_pos.isEqual(Position::npos.getRow(), Position::npos.getCol())) {
        pos = next_pos;
    }
    nums_move++;
}










string Watson::str() const {
    return "Watson[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}

// class Criminal

Criminal::Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson)
: Character(index, init_pos, map, "Criminal"), watson(watson), sherlock(sherlock), valid_move(0), pre_position(Position::npos) {}


int Criminal::dis_to_S_and_W(int r, int c) {
    Position s_urr = sherlock->getCurrentPosition();
    Position w_urr = watson->getCurrentPosition();

    int dissherlock = abs(r - s_urr.getRow()) + abs(c - s_urr.getCol());
    int diswatson = abs(r - w_urr.getRow()) + abs(c - w_urr.getCol());
    
    return dissherlock + diswatson;
}


Position Criminal::getNextPosition() {
    Position nex_pos = pos;
    
    int r = pos.getRow();
    int c = pos.getCol();
    int max_value = 0;

    if(dis_to_S_and_W(r, c + 1) >= max_value) {
        if(map->isValid(Position(r, c + 1), this)) {
            max_value = dis_to_S_and_W(r, c + 1);
            nex_pos = Position(r, c + 1);
        }
    }
    if(dis_to_S_and_W(r + 1, c) >= max_value) {
        if(map->isValid(Position(r + 1, c), this)) {
            max_value = dis_to_S_and_W(r + 1, c);
            
            nex_pos = Position(r + 1, c);
        }
    }
    if(dis_to_S_and_W(r, c - 1) >= max_value) {
        if(map->isValid(Position(r, c - 1), this)) {
            max_value = dis_to_S_and_W(r, c - 1);
            nex_pos = Position(r, c - 1);
        }
    }

    if(dis_to_S_and_W(r - 1, c) >= max_value) {
        if(map->isValid(Position(r - 1, c), this)) {
            max_value = dis_to_S_and_W(r - 1, c);
            nex_pos = Position(r - 1, c);
        }
    }

    if(nex_pos.isEqual(pos.getRow(), pos.getCol())) {
        return Position::npos;
    }    
    return nex_pos;
}

void Criminal::move() {
    Position next_pos = getNextPosition();
    if(!next_pos.isEqual(Position::npos.getRow(), Position::npos.getCol())) {
        pre_position = pos;
        pos = next_pos;   
        valid_move++;       
    }
}

Position Criminal::getPrePosition() {
    return pre_position;
}

int Criminal::getValidMove() const {
    return valid_move;
}



string Criminal::str() const {
    return "Criminal[index=" + to_string(index) + ";pos=" + pos.str() + "]";
}



// class ArrayMovingObject

ArrayMovingObject::ArrayMovingObject(int capacity) : count(0), capacity(capacity) {
    arr_mv_objs = new MovingObject*[capacity];
}

ArrayMovingObject::~ArrayMovingObject() {
    delete[] arr_mv_objs;
}

bool ArrayMovingObject::isFull() const {
    if(count == capacity) return true;
    return false;
}

bool ArrayMovingObject::add(MovingObject *mv_obj) {
    if(isFull()) return false;
    arr_mv_objs[count++] = mv_obj;
    return true;
}

MovingObject* ArrayMovingObject::get(int index) const {
    return arr_mv_objs[index];
}

int ArrayMovingObject::size() const {
    return count;
}

string ArrayMovingObject::str() const {
    ostringstream oss;
    oss << "ArrayMovingObject[count=" << to_string(count) << ";capacity=" << to_string(capacity) << ";";
    for (int i = 0; i < count; i++) {
        oss << arr_mv_objs[i]->str();
        if (i < count - 1) {
            oss << ";";
        }
    }
    oss << "]";

    return oss.str();
    
}

/*-----------class Configuration----------*/

Configuration::Configuration(const string &filepath)
{
    ifstream file(filepath);
    string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            istringstream iss(line);
            string key, value;
            getline(iss, key, '=');
            getline(iss, value);
            
            if (key == "MAP_NUM_ROWS")
                map_num_rows = stoi(value);
            
            else if (key == "MAX_NUM_MOVING_OBJECTS")
                max_num_moving_objects = stoi(value);
            else if (key == "MAP_NUM_COLS")
                map_num_cols = stoi(value);
            else if (key == "ARRAY_WALLS")
            {
                string subValue = value.substr(1, value.size() - 2);
                num_walls = 0;
                int pos = 0;
                while ((pos = subValue.find('(', pos)) != string::npos)
                {
                    int endPos = subValue.find(')', pos);
                    num_walls++;

                    pos = endPos + 1;
                }
                pos = 0;
                int idx = 0;
                arr_walls = new Position[num_walls];
                while ((pos = subValue.find('(', pos)) != string::npos)
                {
                    int endPos = subValue.find(')', pos);
                    char c;
                    int row, col;
                    string substr = subValue.substr(pos, endPos - pos + 1);
                    stringstream subStream(substr);
                    subStream >> c >> row >> c >> col >> c;
                    arr_walls[idx].setRow(row);
                    arr_walls[idx].setCol(col);
                    idx++;
                    pos = endPos + 1;
                }
            }
            else if (key == "ARRAY_FAKE_WALLS")
            {
                string subValue = value.substr(1, value.size() - 2);
                num_fake_walls = 0;
                int pos = 0;
                while ((pos = subValue.find('(', pos)) != string::npos)
                {
                    int endPos = subValue.find(')', pos);
                    num_fake_walls++;

                    pos = endPos + 1;
                }
                pos = 0;
                int idx = 0;
                arr_fake_walls = new Position[num_fake_walls];
                while ((pos = subValue.find('(', pos)) != string::npos)
                {
                    int endPos = subValue.find(')', pos);
                    char c;
                    int row, col;
                    string substr = subValue.substr(pos, endPos - pos + 1);
                    stringstream subStream(substr);
                    subStream >> c >> row >> c >> col >> c;
                    arr_fake_walls[idx].setRow(row);
                    arr_fake_walls[idx].setCol(col);
                    idx++;
                    pos = endPos + 1;
                }
            }
            else if (key == "SHERLOCK_MOVING_RULE")
                sherlock_moving_rule = value;
            else if (key == "SHERLOCK_INIT_POS") {
                int row, col;
                char c;
                istringstream stream(value);
                stream >> c >> row >> c >> col >> c;
                sherlock_init_pos.setCol(col);
                sherlock_init_pos.setRow(row);
            }

            else if (key == "SHERLOCK_INIT_HP")
                sherlock_init_hp = stoi(value);
            else if (key == "SHERLOCK_INIT_EXP")
                sherlock_init_exp = stoi(value);
            else if (key == "WATSON_MOVING_RULE")
                watson_moving_rule = value;
            else if (key == "WATSON_INIT_POS") {
                int row, col;
                char c;
                istringstream stream(value);
                stream >> c >> row >> c >> col >> c;
                watson_init_pos.setRow(row);
                watson_init_pos.setCol(col);
            }
            else if (key == "WATSON_INIT_HP")
                watson_init_hp = stoi(value);
            else if (key == "WATSON_INIT_EXP")
                watson_init_exp = stoi(value);
            else if (key == "CRIMINAL_INIT_POS") {
                int row, col;
                char c;
                istringstream stream(value);
                stream >> c >> row >> c >> col >> c;
                criminal_init_pos.setCol(col);
                criminal_init_pos.setRow(row);
            }
            else if (key == "NUM_STEPS")
                num_steps = stoi(value);
        }
        file.close();
    }
}

Configuration::~Configuration()
{
    delete[] arr_walls;
    delete[] arr_fake_walls;
}





string Configuration::str() const
{
    ostringstream oss;
    oss << "Configuration[" << "\n";
    oss << "MAP_NUM_ROWS=" << map_num_rows << "\n";
    oss << "MAP_NUM_COLS=" << map_num_cols << "\n";
    oss << "MAX_NUM_MOVING_OBJECTS=" << max_num_moving_objects << "\n";
    oss << "NUM_WALLS=" << num_walls << "\n";
    oss << "ARRAY_WALLS=[";
    for (int i = 0; i < num_walls; ++i) {
        oss << "(" << arr_walls[i].getRow() << "," << arr_walls[i].getCol() << ")";
        if (i != num_walls - 1) oss << ";";
    }
    oss << "]" << "\n";
    oss << "NUM_FAKE_WALLS=" << num_fake_walls << "\n";
    oss << "ARRAY_FAKE_WALLS=[";
    for (int i = 0; i < num_fake_walls; ++i) {
        oss << "(" << arr_fake_walls[i].getRow() << "," << arr_fake_walls[i].getCol() << ")";
        if (i != num_fake_walls - 1) oss << ";";
    }
    oss << "]" << "\n";
    oss << "SHERLOCK_MOVING_RULE=" << sherlock_moving_rule << "\n";
    oss << "SHERLOCK_INIT_POS=" << sherlock_init_pos.str() << "\n";
    oss << "SHERLOCK_INIT_HP=" << sherlock_init_hp << "\n";
    oss << "SHERLOCK_INIT_EXP=" << sherlock_init_exp << "\n";
    oss << "WATSON_MOVING_RULE=" << watson_moving_rule << "\n";
    oss << "WATSON_INIT_POS=" << watson_init_pos.str() << "\n";
    oss << "WATSON_INIT_HP=" << watson_init_hp << "\n";
    oss << "WATSON_INIT_EXP=" << watson_init_exp << "\n";
    oss << "CRIMINAL_INIT_POS=" << criminal_init_pos.str() << "\n";
    oss << "NUM_STEPS=" << num_steps << "\n";
    oss << "]" << "\n";
    return oss.str();
}

// class Robot

Robot::Robot(RobotType robot_type, int index, const Position pos, Map * map) : MovingObject(index, pos, map), robot_type(robot_type) {
    int i = pos.getRow();
    int j = pos.getCol();
    int s = i * j;
    while(s >= 10) {
        int n = 0;
        while(s != 0) {
            n += s % 10;
            s /= 10;
        }
        s = n;
    }
    if(s == 0 || s == 1) {
        item = new MagicBook();
    }
    else if(s == 2 || s == 3) {
        item = new EnergyDrink();
    }
    else if(s == 4 || s == 5) {
        item = new FirstAid();
    }
    else if(s == 6 || s == 7) {
        item = new ExcemptionCard();
    }
    else {
        int t = (i * 11 + j) % 4;
        string challenge;
        switch (t) {
        case 0:
            challenge = "RobotS";
            break;
        case 1:
            challenge = "RobotC";
            break;
        case 2:
            challenge = "RobotSW";
            break;
        case 3:
            challenge = "all";
            break;
        }
        item = new PassingCard(challenge);
    }
}
Robot::~Robot(){
    delete item;
}

BaseItem* Robot::getItem() {
    BaseItem* bua = nullptr;
    if(item->item_type == MAGIC_BOOK) {
        bua = new MagicBook();
    }


    else if(item->item_type == ENERGY_DRINK) {
        bua = new EnergyDrink();
    }
    else if(item->item_type == FIRST_AID) {
        bua = new FirstAid();
    }
    else if(item->item_type == EXCEMPTION_CARD) {
        bua = new ExcemptionCard();
    } 



    else {
        PassingCard* pass = dynamic_cast<PassingCard*>(item);
        bua = new PassingCard(pass->challenge);
    }
    return bua;
    
}

// class RobotC

RobotC::RobotC(int index , const Position & init_pos , Map * map , Criminal *criminal)
: Robot(C, index, init_pos, map) {
    this->criminal = criminal;
}



RobotC::~RobotC() {}




Position RobotC::getNextPosition() {
    if(map->isValid(criminal->getPrePosition(), this)) return criminal->getPrePosition();
    return Position::npos;
} 

void RobotC::move() {
    Position next_pos = getNextPosition();
    if(!next_pos.isEqual(Position::npos.getRow(), Position::npos.getCol())) pos = next_pos;
}




int RobotC::getDistance(Sherlock* sherlock) const {
    Position curr_pos = sherlock->getCurrentPosition();
    int dix = abs(curr_pos.getRow() - pos.getRow());
    int diy = abs(curr_pos.getCol() - pos.getCol());
    return dix + diy;
}

int RobotC::getDistance(Watson* watson) const {
    Position curr_pos = watson->getCurrentPosition();
    int dis_x = abs(curr_pos.getRow() - pos.getRow());
    int dis_y = abs(curr_pos.getCol() - pos.getCol());
    return dis_x + dis_y;
}





string RobotC::str() const {
    return "Robot[pos=" + pos.str() + ";type=C;dist=" + "]"; 
}







// class RobotS

RobotS::RobotS(int index , const Position & init_pos , Map * map , Criminal *criminal, Sherlock* sherlock)
: Robot(S, index, init_pos, map) {
    this->criminal = criminal;
    this->sherlock = sherlock;
}
RobotS::~RobotS() {}

Position RobotS::getNextPosition() {
   Position next_pos = pos;
    int min_dis = sherlock->getDistanceToPosition(pos) + 3;
    int row = pos.getRow();
    int col = pos.getCol();
    for(int c = col - 1; c <= col + 1; c++) {
        for(int r = row; r <= row + 1; r++) {
            int distance = abs(row - r) + abs(col - c);
            if(distance == 1) {
                if(sherlock->getDistanceToPosition(Position(r, c)) <= min_dis) {
                    if(map->isValid(Position(r, c), this)) {
                        min_dis = sherlock->getDistanceToPosition(Position(r, c));
                        next_pos = Position(r, c);
                    }
                }  
            }
        }
    }


    if(sherlock->getDistanceToPosition(Position(row - 1, col)) <= min_dis) {
        if(map->isValid(Position(row - 1, col), this)) {
            min_dis = sherlock->getDistanceToPosition(Position(row - 1, col));
            next_pos = Position(row - 1, col);
        }
    }

    if(next_pos.isEqual(pos.getRow(), pos.getCol())) {
        return Position::npos;    
    }
    return next_pos;
    
}




void RobotS::move() {
    Position next_pos = getNextPosition();
    if(!next_pos.isEqual(Position::npos.getRow(), Position::npos.getCol())) pos = next_pos;

}



int RobotS::getDistance() const {
    Position curr_pos = sherlock->getCurrentPosition();
    int dis_x = abs(curr_pos.getRow() - pos.getRow());
    int dis_y = abs(curr_pos.getCol() - pos.getCol());
    return dis_x + dis_y;
}
string RobotS::str() const {
    int distance = getDistance();
    return "Robot[pos=" + pos.str() + ";type=S;dist=" + to_string(distance) + "]"; 
}


/*----------class RobotW----------*/

RobotW::RobotW(int index , const Position & init_pos , Map * map , Criminal *criminal, Watson* watson)
: Robot(W, index, init_pos, map), criminal(criminal), watson(watson) {
    this->criminal = criminal;
    this->watson = watson;
};
RobotW::~RobotW() {}

Position RobotW::getNextPosition() {
    Position next_pos = pos;
    int min_dis = watson->getDistanceToPosition(pos) + 3;
    int row = pos.getRow();
    int col = pos.getCol();
    for(int c = col - 1; c <= col + 1; c++) {
        for(int r = row; r <= row + 1; r++) {
            int distance = abs(row - r) + abs(col - c);
            if(distance == 1) {
                if(watson->getDistanceToPosition(Position(r, c)) <= min_dis) {
                    if(map->isValid(Position(r, c), this)) {
                        min_dis = watson->getDistanceToPosition(Position(r, c));
                        next_pos = Position(r, c);
                    }
                }  
            }
        }
    }
    if(watson->getDistanceToPosition(Position(row - 1, col)) <= min_dis) {
        if(map->isValid(Position(row - 1, col), this)) {
            min_dis = watson->getDistanceToPosition(Position(row - 1, col));
            next_pos = Position(row - 1, col);
        }
    }

    if(next_pos.isEqual(pos.getRow(), pos.getCol())) {
        return Position::npos;
    }
    
    return next_pos;
    

}

void RobotW::move() {
    Position nex_pos = getNextPosition();
    int row = nex_pos.getRow();
    int col = nex_pos.getCol();
    if(!Position::npos.isEqual(row, col)) pos = nex_pos;

}

int RobotW::getDistance() const {
    Position curr_pos = watson->getCurrentPosition();
    int dis_x = abs(curr_pos.getRow() - pos.getRow());
    int dis_y = abs(curr_pos.getCol() - pos.getCol());
    return dis_x + dis_y;
}
string RobotW::str() const {
    int distance = getDistance();
    return "Robot[pos=" + pos.str() + ";type=W;dist=" + to_string(distance) + "]"; 
}

// class RobotSW

RobotSW::RobotSW(int index , const Position & init_pos , Map * map , Criminal *criminal, Sherlock* sherlock, Watson* watson)
: Robot(SW, index, init_pos, map), criminal(criminal), watson(watson), sherlock(sherlock) {
    this->criminal = criminal;
    this->watson = watson;
    this->sherlock = sherlock;
};
RobotSW::~RobotSW() {}

Position RobotSW::getNextPosition() {
    Position next_pos = pos;
    int min_distance = sherlock->getDistanceToPosition(pos) + watson->getDistanceToPosition(pos) + 8;
    int r = pos.getRow();
    int c = pos.getCol();

    for (int i = r - 2; i < r + 3; i++)
    {
        for (int j = c; j < c + 3; j++)
        {
            Position current_pos(i, j);
            int distance = abs(r - i) + abs(c - j);
            if (distance == 2)
            {
                int total_distance = sherlock->getDistanceToPosition(current_pos) + watson->getDistanceToPosition(current_pos);
                
                if(map->isValid(current_pos, this)) {
                    if(total_distance < min_distance) {
                        min_distance = total_distance;
                        next_pos = current_pos;
                    }
                }
            }
        }
    }


    for(int i = r + 1; i >= r - 1; i--) {
        for(int j = c - 2; j < c; j++) {
            Position current_pos(i, j);
            int distance = abs(r - i) + abs(c - j);
            if(distance == 2) {
                int total_distance = sherlock->getDistanceToPosition(current_pos) + watson->getDistanceToPosition(current_pos);
                if(map->isValid(current_pos, this)) {
                    if(total_distance < min_distance) {
                        min_distance = total_distance;
                        next_pos = current_pos;
                    }
                }
            }
        }
    }
    

    if(pos.isEqual(next_pos.getRow(), next_pos.getCol())) {
        return Position::npos;    
    }
    
    return next_pos;
}

void RobotSW::move() {
    Position nex_pos = getNextPosition();
    int row = nex_pos.getRow();
    int col = nex_pos.getCol();
    if(!Position::npos.isEqual(row, col)) pos = nex_pos;

}

int RobotSW::getDistance() const {
    Position s_pos = sherlock->getCurrentPosition();
    Position w_pos = watson->getCurrentPosition();
    int x1 = abs(s_pos.getRow() - pos.getRow());
    int y1 = abs(s_pos.getCol() - pos.getCol());
    int sherlock_dis = x1 + y1;
    int x2 = abs(w_pos.getRow() - pos.getRow());
    int y2 = abs(w_pos.getCol() - pos.getCol());
    int watson_dis = x2 + y2;
    return sherlock_dis + watson_dis;
}
string RobotSW::str() const {
    int distance = getDistance();
    return "Robot[pos=" + pos.str() + ";type=SW;dist=" + to_string(distance) + "]"; 
}

// class BaseItem

BaseItem::BaseItem(ItemType item_type) {
    this->item_type = item_type;
    this->next = nullptr;
}
BaseItem::~BaseItem() {}

// class MagicBook


bool MagicBook::canUse(Character* obj, Robot* robot) {
    if(obj->exp <= 350) return true;
    return false;    
}
void MagicBook::use(Character* obj, Robot* robot){
    obj->setExp(ceil(obj->exp*1.25));
}

// class EneryDrink

bool EnergyDrink::canUse(Character* obj, Robot* robot){
    if(obj->hp <= 100) return true;
    return false;
}

void EnergyDrink::use(Character* obj, Robot* robot){
    obj->setHp(ceil(obj->hp*1.2));
}
// class FirstAid

bool FirstAid::canUse(Character* obj, Robot* robot){
    if(obj->hp <= 100 || obj->exp <= 350) return true;
    return false;
}
void FirstAid::use(Character* obj, Robot* robot){
    obj->setHp(ceil(obj->hp * 1.5));
}

// class ExcemptionCard

bool ExcemptionCard::canUse(Character* obj, Robot* robot){
    int curr_hp = obj->hp;
    if(obj->getName() == "Sherlock" && curr_hp % 2 != 0) return true;
    return false;
}
void ExcemptionCard::use(Character* obj, Robot* robot){

}

// class PassingCard

PassingCard::PassingCard(const string & challenge) : BaseItem(PASSING_CARD) {
    this->challenge = challenge;
}
PassingCard::~PassingCard() {}

bool PassingCard::canUse(Character* obj, Robot* robot){
    int curr_hp = obj->hp;
    if(obj->getName() == "Watson" && curr_hp % 2 == 0) return true;
    return false;
}
void PassingCard::use(Character* obj, Robot* robot){
    if(robot == nullptr) return;

    if(challenge == "all") return;
    if(challenge == "robotC" && robot->getRobotType() == C) return;
    if(challenge == "robotSW" && robot->getRobotType() == SW) return;
    if(challenge == "robotS" && robot->getRobotType() == S) return;
    obj->setExp(obj->exp - 50);
}


// class SherlockBag
 
SherlockBag::SherlockBag(Sherlock* sherlock) : BaseBag(sherlock), head(nullptr), count(0) {
    
}
SherlockBag::~SherlockBag(){}
int SherlockBag::numsOfItem(ItemType itemType) {
    BaseItem* current = head;
    int n = 0;
    while(current != nullptr) {
        if(current->item_type == itemType) n++;
        current = current->next;
    }
    return n;
}
bool SherlockBag::insert(BaseItem* item) {
    if(count == 13) return false;
    if(head == nullptr) {
        head = item;
    }
    else {
        BaseItem* temp = head;
        item->next = temp;
        head = item;
    }
    count++;
    return true;
}   
BaseItem * SherlockBag::get() {
    if(head == nullptr) return nullptr;
    BaseItem* cur = head;
    BaseItem* prue = nullptr;  
    while(cur != nullptr) {
        prue = cur;
        cur = cur->next;
    }    
    if(cur == nullptr) return nullptr;
    if(cur == head) {
        head = head->next;
    }
    else {
        prue->next = head;
        BaseItem* tmp = head->next;
        head->next = cur->next;
        cur->next = tmp;
        head = cur;
        head = head->next;
    }
    count--;
    return cur;
    
}
BaseItem * SherlockBag::get(ItemType itemType) {
    if(head == nullptr) return nullptr;
    BaseItem* cuirr = head;
    BaseItem* previ = nullptr;  
    while(cuirr != nullptr && cuirr->item_type != itemType) {
        previ = cuirr;
        cuirr = cuirr->next;
    }    
    if(cuirr == nullptr) return nullptr;
    if(cuirr == head) head = head->next;
    else {
        previ->next = head;
        BaseItem* tmp = head->next;
        head->next = cuirr->next;
        cuirr->next = tmp;
        head = cuirr;
        head = head->next;
    }
    count--;
    return cuirr;
}
BaseItem * SherlockBag::canUseItem() {
    if(head == nullptr) return nullptr;
    BaseItem* curir = head;
    BaseItem* prei = nullptr;
    int idx = 0;
    while(curir != nullptr) {
        if(curir->item_type == MAGIC_BOOK || curir->item_type == ENERGY_DRINK
            || curir->item_type == FIRST_AID) {
            if(curir->canUse(obj, nullptr)) {
                break;
            }
        }
        prei = curir;
        curir = curir->next;
    }    
    if(curir == nullptr) return nullptr;
    if(curir == head) {
        head = head->next;
    }
    else {
        prei->next = head;
        BaseItem* tmp = head->next;
        head->next = curir->next;
        curir->next = tmp;
        head = curir;
        head = head->next;
        
    }
    count--;
    return curir;
    
}
string SherlockBag::str() const {
    ostringstream oss;
    oss << "Bag[count=" << to_string(count) << ";";
    BaseItem* item = head;
    int index = 0;
    while(item != nullptr) {
        if(item->item_type == MAGIC_BOOK) {
            oss << "MagicBook";
        }
        else if(item->item_type == ENERGY_DRINK) {
            oss << "EnergyDrink";
        }
        else if(item->item_type == FIRST_AID) {
            oss << "FirstAid";
        }
        else if(item->item_type == EXCEMPTION_CARD) {
            oss << "ExcemptionCard";
        }
        else {
            oss << "PassingCard";
        }
        if(index == count - 1) {
            break;
        }
        oss << ",";
        index++;
        item = item->next;
    }
    oss << "]";
    return oss.str();
}


// class WatsonBag

WatsonBag::WatsonBag(Watson* watson) : BaseBag(watson), head(nullptr), count(0){}
WatsonBag::~WatsonBag(){}

int WatsonBag::numsOfItem(ItemType itemType) const {
    BaseItem* temp = head;
    int idx = 0;
    while(temp != nullptr) {
        if(temp->item_type == itemType) idx++;
        temp = temp->next;
    }
    return idx;
}

bool WatsonBag::insert(BaseItem* item) {
    if(count == 15) return false;
    if(head == nullptr) {
        head = item;
    }
    else {
        BaseItem* temp = head;
        item->next = temp;
        head = item;
    }
    count++;
    return true;

}
BaseItem * WatsonBag::get() {
    if(head == nullptr) return nullptr;
    BaseItem* curr = head;
    BaseItem* pre = nullptr;  
    while(curr != nullptr && !curr->canUse(obj, nullptr)) {
        pre = curr;
        curr = curr->next;
    }    
    if(curr == nullptr) return nullptr;
    if(curr == head) {
        head = head->next;
    }
    else {
        pre->next = head;
        BaseItem* tmp = head->next;
        head->next = curr->next;
        curr->next = tmp;
        head = curr;
        head = head->next;
    }
    count--;
    return curr;
}
BaseItem * WatsonBag::get(ItemType itemType) {
   if(head == nullptr) return nullptr;
    BaseItem* curr = head;
    BaseItem* pre = nullptr;  
    while(curr != nullptr && curr->item_type != itemType) {
        pre = curr;
        curr = curr->next;
    }    
    if(curr == nullptr) return nullptr;
    if(curr == head) head = head->next;
    else {
        pre->next = head;
        BaseItem* tmp = head->next;
        head->next = curr->next;
        curr->next = tmp;
        head = curr;
        head = head->next;
    }
    count--;
    return curr;

}

BaseItem * WatsonBag::canUseItem() {
    if(head == nullptr) return nullptr;
    BaseItem* curr = head;
    BaseItem* pre = nullptr;
    int idx = 0;
    while(curr != nullptr) {
        if(curr->item_type == MAGIC_BOOK || curr->item_type == ENERGY_DRINK
            || curr->item_type == FIRST_AID) {
            if(curr->canUse(obj, nullptr)) {
                break;
            }
        }
        pre = curr;
        curr = curr->next;
    }    
    if(curr == nullptr) return nullptr;
    if(curr == head) {
        head = head->next;
    }
    else {
        pre->next = head;
        BaseItem* tmp = head->next;
        head->next = curr->next;
        curr->next = tmp;
        head = curr;
        head = head->next;
        
    }
    count--;
    return curr;
    
}
string WatsonBag::str() const {
    ostringstream oss;
    oss << "Bag[count=" << to_string(count) << ";";
    BaseItem* item = head;
    int index = 0;
    while(item != nullptr) {
        if(item->item_type == MAGIC_BOOK) {
            oss << "MagicBook";
        }
        else if(item->item_type == ENERGY_DRINK) {
            oss << "EnergyDrink";
        }
        else if(item->item_type == FIRST_AID) {
            oss << "FirstAid";
        }
        else if(item->item_type == EXCEMPTION_CARD) {
            oss << "ExcemptionCard";
        }
        else {
            oss << "PassingCard";
        }
        if(index == count - 1) {
            break;
        }
        oss << ",";
        index++;
        item = item->next;
    }
    oss << "]";
    return oss.str();
}

// class StudyPinkProgram

void StudyPinkProgram::createRobotMove() {
    int valid_count_step = criminal->getValidMove();
     int index = arr_mv_objs->size();
    if(valid_count_step == 3) {
        Position pre_pos = criminal->getPrePosition();
        RobotC* robotC = new RobotC(index, pre_pos, map, criminal);
        arr_mv_objs->add(robotC);

    }
    else if(valid_count_step % 3 == 0) {
        Position pos = criminal->getPrePosition();

        int sherlockDis = sherlock->getDistanceToPosition(pos);
        int watsonDis = watson->getDistanceToPosition(pos);
        if(sherlockDis < watsonDis) {
            
            arr_mv_objs->add(new RobotS(index, pos, map, criminal, sherlock));
        }
        else if(sherlockDis > watsonDis) {
            
            arr_mv_objs->add(new RobotW(index, pos, map, criminal, watson));
        }
        else {
            arr_mv_objs->add(new RobotSW(index, pos, map, criminal, sherlock, watson));
        }        
    }
}

bool StudyPinkProgram::isStop() const {
    if(sherlock->hp == 1 || watson->hp == 1  
        || sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(),
        criminal->getCurrentPosition().getCol())
        || watson->getCurrentPosition().isEqual(criminal->getCurrentPosition().getRow(),
        criminal->getCurrentPosition().getCol())     
    ) {
        return true;
    }

    return false;
}

void StudyPinkProgram::robotConflict(MovingObject* obj) {
    Robot* robot = dynamic_cast<Robot* >(obj);
    Position sherPos = sherlock->getCurrentPosition();
    Position watPos = watson->getCurrentPosition();
    Position robotPos = robot->getCurrentPosition();
    if(robotPos.isEqual(sherPos.getRow(), sherPos.getCol())) {
        BaseItem* canItem1 = sherlockBag->get(EXCEMPTION_CARD);
        bool isUsed = false;
        if(canItem1 != nullptr) {
            if(canItem1->canUse(sherlock, robot)) {
                canItem1->use(sherlock, robot);
                isUsed = true;
            }
        }
        if(robot->getRobotType() == W) sherlockBag->insert(robot->getItem());
        else if (robot->getRobotType() == C) {
            if(sherlock->exp > 500) sherlock->editPosition(criminal->getCurrentPosition());
            else sherlockBag->insert(robot->getItem());
        }
        
        else if (robot->getRobotType() == S) {
            if(sherlock->exp > 400) sherlockBag->insert(robot->getItem());
            else {
                if(!isUsed) {
                    int new_exp = ceil(sherlock->exp * 0.9);
                    sherlock->setExp(new_exp);
                }
            }
        }
        else
        {
            if(sherlock->exp > 300 && sherlock->hp > 335) sherlockBag->insert(robot->getItem());
            else {
                if(!isUsed) {
                    int new_hp = ceil(sherlock->hp * 0.85);
                    int new_exp = ceil(sherlock->exp * 0.85);
                    sherlock->setHp(new_hp);
                    sherlock->setExp(new_exp);
                } 
            
            }
        }
        BaseItem* canItem2 = sherlockBag->canUseItem();
        if(canItem2 != nullptr) canItem2->use(sherlock, nullptr);
    }
    if(robotPos.isEqual(watPos.getRow(), watPos.getCol())) {
        BaseItem *canItem1 = watsonBag->get(PASSING_CARD);
        bool isUsed = false;
        if (canItem1 != nullptr) {
            if(canItem1->canUse(watson, robot)) {
                canItem1->use(watson, robot);
                isUsed = true;
            }
        }
            
        if (robot->getRobotType() == W)
        {
            if (isUsed) watsonBag->insert(robot->getItem());
            else
            {
                if (watson->hp > 350) watsonBag->insert(robot->getItem());
                else {
                    int new_hp = ceil(watson->hp - watson->hp * 0.05);
                    watson->setHp(new_hp);
                }
            }
        }
        else if (robot->getRobotType() == C) watsonBag->insert(robot->getItem());
        else if (robot->getRobotType() == S) {}
        else
        {
            if (isUsed) watsonBag->insert(robot->getItem());
            else
            {
                if (watson->exp > 600 && watson->hp > 165) watsonBag->insert(robot->getItem());
                else {
                    int new_hp = ceil(watson->hp * 0.85);
                    int new_exp = ceil(watson->exp * 0.85);
                    watson->setHp(new_hp);
                    watson->setExp(new_exp);
                }
            }
        }
        BaseItem *canItem2 = watsonBag->canUseItem();
        if (canItem2 != nullptr) canItem2->use(watson, nullptr);    
    }
}

void StudyPinkProgram::sherlockConflict() {
    Position sherlockPos = sherlock->getCurrentPosition();
    for(int i = 0; i < arr_mv_objs->size(); i++) {
        Position objectPos = arr_mv_objs->get(i)->getCurrentPosition();
        if(sherlockPos.isEqual(objectPos.getRow(), objectPos.getCol())) {
            if(arr_mv_objs->get(i) == criminal) {
                return;
            }
            if(arr_mv_objs->get(i) == watson) {
                bool isFoundItemSherlock = sherlockBag->str().find("PassingCard") != string::npos;
                bool isFoundItemWatson = watsonBag->str().find("ExcemptionCard") != string::npos;
                if(isFoundItemSherlock && isFoundItemWatson) {
                    int idx_sherlock = sherlockBag->numsOfItem(PASSING_CARD);
                    int idx_watson = watsonBag->numsOfItem(EXCEMPTION_CARD);
                    for(int i = 0; i < idx_sherlock; i++) {
                        BaseItem* item = sherlockBag->get(PASSING_CARD);
                        watsonBag->insert(item);
                    }
                    for(int i = 0; i < idx_watson; i++) {
                        BaseItem* item = watsonBag->get(EXCEMPTION_CARD);
                        sherlockBag->insert(item);
                    }
                }
            }

            bool isRobot = arr_mv_objs->get(i)->str().find("Robot") != string::npos;
            if(isRobot) {
                Robot *robot = dynamic_cast<Robot *>(arr_mv_objs->get(i));
                BaseItem* canItem1 = sherlockBag->get(EXCEMPTION_CARD);
                bool isUsed = false;
                if(canItem1 != nullptr) {
                    if(canItem1->canUse(sherlock, robot)) {
                        canItem1->use(sherlock, robot);
                        isUsed = true;
                    }
                }
                if(robot->getRobotType() == W) sherlockBag->insert(robot->getItem());
                else if (robot->getRobotType() == C) {
                    if(sherlock->exp > 500) sherlock->editPosition(criminal->getCurrentPosition());
                    else sherlockBag->insert(robot->getItem());
                }
                
                else if (robot->getRobotType() == S) {
                    if(sherlock->exp > 400) sherlockBag->insert(robot->getItem());
                    else {
                        if(!isUsed) {
                            int new_exp = ceil(sherlock->exp * 0.9);
                            sherlock->setExp(new_exp);
                        }
                    }
                }
                else
                {
                    if(sherlock->exp > 300 && sherlock->hp > 335) sherlockBag->insert(robot->getItem());
                    else {
                        if(!isUsed) {
                            int new_hp = ceil(sherlock->hp * 0.85);
                            int new_exp = ceil(sherlock->exp * 0.85);
                            sherlock->setHp(new_hp);
                            sherlock->setExp(new_exp);
                        } 
                    
                    }
                }
                BaseItem* canItem2 = sherlockBag->canUseItem();
                if(canItem2 != nullptr) canItem2->use(sherlock, nullptr);
            }
        } 
    }
}

void StudyPinkProgram::watsonConflict() {
    Position watsonPos = watson->getCurrentPosition();
    for(int i = 0; i < arr_mv_objs->size(); i++) {
        Position objectPos = arr_mv_objs->get(i)->getCurrentPosition();
        if(watsonPos.isEqual(objectPos.getRow(), objectPos.getCol())) {
            if(arr_mv_objs->get(i) == criminal) {
                return;
            }
            if(arr_mv_objs->get(i) == sherlock) {
                bool isFoundItemSherlock = sherlockBag->str().find("PassingCard") != string::npos;
                bool isFoundItemWatson = watsonBag->str().find("ExcemptionCard") != string::npos;
                if(isFoundItemSherlock && isFoundItemWatson) {
                    int idx_sherlock = sherlockBag->numsOfItem(PASSING_CARD);
                    int idx_watson = watsonBag->numsOfItem(EXCEMPTION_CARD);
                    for(int i = 0; i < idx_sherlock; i++) {
                        BaseItem* item = sherlockBag->get(PASSING_CARD);
                        watsonBag->insert(item);
                    }
                    for(int i = 0; i < idx_watson; i++) {
                        BaseItem* item = watsonBag->get(EXCEMPTION_CARD);
                        sherlockBag->insert(item);
                    }
                }
            }
            bool isRobot = arr_mv_objs->get(i)->str().find("Robot") != string::npos;
            if(isRobot) {
                Robot *robot = dynamic_cast<Robot *>(arr_mv_objs->get(i));
                BaseItem *canItem1 = watsonBag->get(PASSING_CARD);
                bool isUsed = false;
                if (canItem1 != nullptr) {
                    if(canItem1->canUse(watson, robot)) {
                        canItem1->use(watson, robot);
                        isUsed = true;
                    }
                }
                    
                if (robot->getRobotType() == W)
                {
                    if (isUsed) watsonBag->insert(robot->getItem());
                    else
                    {
                        if (watson->hp > 350) watsonBag->insert(robot->getItem());
                        else {
                            int new_hp = ceil(watson->hp - watson->hp * 0.05);
                            watson->setHp(new_hp);
                        }
                    }
                }
                else if (robot->getRobotType() == C) watsonBag->insert(robot->getItem());
                else if (robot->getRobotType() == S) {}
                else
                {
                    if (isUsed) watsonBag->insert(robot->getItem());
                    else
                    {
                        if (watson->exp > 600 && watson->hp > 165) watsonBag->insert(robot->getItem());
                        else {
                            int new_hp = ceil(watson->hp * 0.85);
                            int new_exp = ceil(watson->exp * 0.85);
                            watson->setHp(new_hp);
                            watson->setExp(new_exp);
                        }
                    }
                }
                BaseItem *canItem2 = watsonBag->canUseItem();
                if (canItem2 != nullptr) canItem2->use(watson, nullptr);
            }
        } 
    }
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////

