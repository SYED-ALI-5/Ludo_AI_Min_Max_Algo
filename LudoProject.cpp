#include <iostream>
#include <vector>
#define SIZE 4
#include <time.h>
#include <thread>
#include <chrono>
#include <climits>

using namespace std;

enum Turn
{

    COM,
    USER
};

enum pieceStatus
{

    INGAME,
    OUTGAME
};
class piece
{

    string name;
    int currentindex;
    pieceStatus p_status;

public:
    piece() {}
    piece(string Name)
    {
        name = Name;
        currentindex = -1;
        p_status = OUTGAME;
    }

    string getname()
    {
        return name;
    }
    int getPieceindex()
    {
        return currentindex;
    }

    void setIndex(int index)
    {
        currentindex = index;
    }
    void setpieceStatus(pieceStatus S_t)
    {
        p_status = S_t;
    }
    pieceStatus getpieceStatus()
    {
        return p_status;
    }
};
class Stack
{
    piece *arr;
    int top;

public:
    Stack()
    {
        arr = new piece[SIZE];
        top = -1;
    }

    void push(piece x)
    {
        if (top == SIZE - 1)
        {
            cout << "Stack overflow!!" << endl;
        }
        else
        {
            top++;
            arr[top] = x;
        }
    }

    piece pop()
    {
        if (top == -1)
        {
            cout << "Stack underflow!!" << endl;
        }
        else
        {
            piece popped = arr[top];
            top--;
            return popped;
        }
    }
    bool empty()
    {
        return top == -1;
    }

    void Display()
    {
        if (empty())
        {
            cout << " " << endl;
            return;
        }
        for (int i = top; i >= 0; i--)
        {
            cout << arr[i].getname() << " ";
        }
        cout << endl;
    }
    bool Full()
    {
        return top == SIZE - 1;
    }
};
class Player
{
    string name;
    vector<piece> pieces;

public:
    Player(string Name)
    {
        name = Name;
        pieces = vector<piece>();
    }

    piece getPlayerPiece(string name)
    {
        piece Selectedpiece;
        for (int i = 0; i < pieces.size(); i++)
        {
            if (pieces[i].getname() == name)
            {
                Selectedpiece = pieces[i];
            }
        }
        return Selectedpiece;
    }
    void Addpiece(piece p)
    {
        pieces.push_back(p);
    }
};

enum CellStatus
{

    OCCUPIED,
    FREE
};

enum CellType
{
    OPEN,
    HIDE
};

class cell
{
    int CellIndex;
    CellStatus status;
    CellType type;
    piece Peice;

public:
    cell() {}
    cell(int index, CellType T)
    {
        CellIndex = index;
        status = FREE;
        type = T;
    }

    CellType getType()
    {
        return type;
    }
    CellStatus getStatus()
    {
        return status;
    }
    int getCellIndex()
    {
        return CellIndex;
    }
    piece &getCellPiece()
    {
        return Peice;
    }

    void Setpiece(piece P)
    {
        Peice = P;
        status = OCCUPIED;
    }
    void removepiece()
    {
        Peice = piece();
        status = FREE;
    }
    void Setstatus(CellStatus St)
    {

        status = St;
    }
};

class Board
{

    vector<cell> cells;

public:
    Board()
    {
        initializeBoard();
    }

    void initializeBoard()
    {
        for (int i = 0; i < 52; i++)
        {
            if (i == 3 || i == 16 || i == 29 || i == 42)
            {
                cells.push_back(cell(i, HIDE));
            }
            else
                cells.push_back(cell(i, OPEN));
        }
        for (int i = 52; i < 72; i++)
        {
            cells.push_back(cell(i, HIDE));
        }
    }
    char Showcell(int index, Turn T)
    {
        if (cells[index].getStatus() == FREE)
        {
            return ' ';
        }
        string piece = cells.at(index).getCellPiece().getname();
        if (T == COM)
        {
            if (piece == "R1")
            {
                return '1';
            }
            if (piece == "R2")
            {
                return '2';
            }
            if (piece == "R3")
            {
                return '3';
            }
            if (piece == "R4")
            {
                return '4';
            }
            else
            {
                return 'G';
            }
        }
        else if (T == USER)
        {
            if (piece == "G1")
                return '1';
            if (piece == "G2")
                return '2';
            if (piece == "G3")
                return '3';
            if (piece == "G4")
                return '4';
            else
                return 'R';
        }
    }

    cell &getcell(int index)
    {
        for (int i = 0; i < cells.size(); i++)
        {
            if (cells[i].getCellIndex() == index)
            {
                return cells[i];
            }
        }
        cout << "Cannot Move !" << endl;
    }

    void SetCell(int index, piece &p)
    {
        for (int i = 0; i < cells.size(); i++)
        {
            if (cells.at(i).getCellIndex() == index)
            {
                cells.at(i).Setpiece(p);
            }
        }
    }
    void StepAhead(piece &p, int Number, Turn T)
    {
        cell &CurrentCell = getcell(p.getPieceindex());
        int NextIndex = p.getPieceindex() + Number;
        if (T == COM)
        {
            if (CurrentCell.getCellIndex() == 51)
            {
                if (Number == 1)
                {
                    NextIndex = 0;
                }
                else if (Number == 2)
                {
                    NextIndex = 1;
                }
                else if (Number >= 3)
                {
                    NextIndex = NextIndex - 2;
                }
            }
        }
        if (T == USER)
        {
            if (NextIndex >= 52)
            {
                NextIndex -= 52;
            }
        }
        if (getcell(NextIndex).getStatus() == FREE)
        {
            CurrentCell.removepiece();
            p.setIndex(NextIndex);
            SetCell(p.getPieceindex(), p);
            cout << p.getname() << ": Stepped Ahead" << endl;
        }
        else if (getcell(NextIndex).getStatus() == OCCUPIED)
        {
            cout << "Cannot Move :(" << endl;
        }
    }

    int killMove(piece &p, int Number, Stack &S, Turn T)
    {
        int Nextindex = p.getPieceindex() + Number;
        cell &NextCell = getcell(Nextindex);
        if (T == COM)
        {
            if (NextCell.getCellPiece().getname() == "R1" || NextCell.getCellPiece().getname() == "R2" || NextCell.getCellPiece().getname() == "R3" || NextCell.getCellPiece().getname() == "R4")
            {
                cout << "Cannot Kill :( " << endl;
                return 0;
            }
        }
        if (T == USER)
        {
            if (NextCell.getCellPiece().getname() == "G1" || NextCell.getCellPiece().getname() == "G2" || NextCell.getCellPiece().getname() == "G3" || NextCell.getCellPiece().getname() == "G4")
            {
                cout << "Cannot Kill :( " << endl;
                return 0;
            }
        }
        cell &CurrentCell = getcell(p.getPieceindex());
        CurrentCell.removepiece();
        p.setIndex(Nextindex);
        piece &tokill = getcell(Nextindex).getCellPiece();
        tokill.setpieceStatus(OUTGAME);
        tokill.setIndex(-1);
        S.push(tokill);
        SetCell(p.getPieceindex(), p);
        return 0;
    }

    int Open(Turn T, piece &p, Stack &S)
    {
        if (T == COM)
        {
            if (getcell(3).getStatus() == OCCUPIED)
            {
                cout << "Cannot Open !" << endl;
                return 0;
            }
            else
            {
                SetCell(3, p);
                p.setIndex(3);
                p.setpieceStatus(INGAME);
                S.pop();
                return 1;
            }
        }
        if (T == USER)
        {
            if (getcell(29).getStatus() == OCCUPIED)
            {
                cout << "Cannot Open !" << endl;
                return 0;
            }
            else
            {
                SetCell(29, p);
                p.setIndex(29);
                p.setpieceStatus(INGAME);
                S.pop();
                return 1;
            }
        }
    }
};
enum Action
{
    NO_Action,
    RUN,
    STEP_AHEAD,
    KILL,
    OPEN_NEW
};
struct GameState
{
    int U1_index;
    int U2_index;
    int U3_index;
    int U4_index;

    int C1_index;
    int C2_index;
    int C3_index;
    int C4_index;

    int Selectedpiece;
    Action action;
    int weight;

    vector<GameState *> Childrens;

public:
    GameState(int U1, int U2, int U3, int U4, int C1, int C2, int C3, int C4, int peice, Action a, int w)
    {
        U1_index = U1;
        U2_index = U2;
        U3_index = U3;
        U4_index = U4;
        C1_index = C1;
        C2_index = C2;
        C3_index = C3;
        C4_index = C4;
        Selectedpiece = peice;
        action = a;
        weight = w;
        Childrens = vector<GameState *>();
    }
};
Action takeAction(piece &p, int i, int Number, Board &LudoBoard)
{
    Action Act = NO_Action;
    switch (i)
    {
    case 1:
    {
        if (Number == 6 && LudoBoard.getcell(3).getStatus() != OCCUPIED)
        {
            Act = OPEN_NEW;
        }
        break;
    }
    case 2:
    {
        for (int j = 1; j <= 10; j++)
        {
            int reverseindex = p.getPieceindex() - i;
            if (reverseindex < 0)
            {
                reverseindex += 52;
            }
            if (LudoBoard.getcell(reverseindex).getStatus() == OCCUPIED)
            {
                Act = RUN;
            }
        }
        break;
    }
    case 3:
    {
        int Next = p.getPieceindex() + Number;
        if (LudoBoard.getcell(Next).getStatus() == OCCUPIED)
        {
            if (LudoBoard.getcell(Next).getCellPiece().getname() == "G1" || LudoBoard.getcell(Next).getCellPiece().getname() == "G2" || LudoBoard.getcell(Next).getCellPiece().getname() == "G3" || LudoBoard.getcell(Next).getCellPiece().getname() == "G4")
            {
                Act = KILL;
            }
        }
        break;
    }
    case 4:
    {
        if (p.getpieceStatus() == INGAME)
        {
            Act = STEP_AHEAD;
        }
    }
    }
    return Act;
}
class Tree
{
    GameState *root;

public:
    Tree(piece &P1, piece &P2, piece &P3, piece &P4)
    {
        root = new GameState(P1.getPieceindex(), P2.getPieceindex(), P3.getPieceindex(), P4.getPieceindex(), 0, 0, 0, 0, 0, NO_Action, 0);
    }
    void BuildTree(piece &p1, piece &p2, piece &p3, piece &p4)
    {
        for (int i = 1; i <= 4; i++)
        {
            root->Childrens.push_back(new GameState(root->U1_index, root->U2_index, root->U3_index, root->U4_index, p1.getPieceindex(), p2.getPieceindex(), p3.getPieceindex(), p4.getPieceindex(), i, NO_Action, 0));
        }
    }
    void Decision(piece &P1, piece &P2, piece &P3, piece &P4, int Number, Board &Ludo)
    {
        piece p = P1;
        int j = 1;
        for (GameState *child : root->Childrens)
        {
            for (int i = 1; i <= 4; i++)
            {
                if (j == 2)
                {
                    p = P2;
                }
                if (j == 3)
                {
                    p = P3;
                }
                if (j == 4)
                {
                    p = P4;
                }
                Action DecidedAct = takeAction(P1, i, Number, Ludo);
                if (DecidedAct != NO_Action)
                {
                    if (DecidedAct == STEP_AHEAD)
                    {
                        child->Childrens.push_back(new GameState(child->U1_index, child->U2_index, child->U3_index, child->U4_index, child->C1_index, child->C2_index, child->C3_index, child->C4_index, child->Selectedpiece, DecidedAct, 10));
                    }
                    if (DecidedAct == OPEN)
                    {
                        child->Childrens.push_back(new GameState(child->U1_index, child->U2_index, child->U3_index, child->U4_index, child->C1_index, child->C2_index, child->C3_index, child->C4_index, child->Selectedpiece, DecidedAct, 20));
                    }
                    if (DecidedAct == KILL)
                    {
                        child->Childrens.push_back(new GameState(child->U1_index, child->U2_index, child->U3_index, child->U4_index, child->C1_index, child->C2_index, child->C3_index, child->C4_index, child->Selectedpiece, DecidedAct, 30));
                    }
                    if (DecidedAct == RUN)
                    {
                        child->Childrens.push_back(new GameState(child->U1_index, child->U2_index, child->U3_index, child->U4_index, child->C1_index, child->C2_index, child->C3_index, child->C4_index, child->Selectedpiece, DecidedAct, 40));
                    }
                }
            }
            j++;
        }
    }

    int DFS(GameState *node)
    {
        if (node->Childrens.empty())
        {
            return node->Selectedpiece;
        }
        int maxWeight = INT_MIN;
        int maxSelectedPiece = -1;

        for (GameState *child : node->Childrens)
        {
            int childSelectedPiece = DFS(child);
            if (child->weight > maxWeight)
            {
                maxWeight = child->weight;
                maxSelectedPiece = childSelectedPiece;
            }
        }
        return maxSelectedPiece;
    }
    int StartDFS()
    {
        DFS(root);
    }
};

void DisplayBoard(Board &LudoBoard, Turn T, Stack &User, Stack &Com)
{

    cout << "--------------------------------------------------------------" << endl;
    cout << "|                        | " << LudoBoard.Showcell(0, T) << " | " << LudoBoard.Showcell(1, T) << " | " << LudoBoard.Showcell(2, T) << " |                       |" << endl;
    cout << "|                        |---|---|===|                       |" << endl;
    cout << "|                        | " << LudoBoard.Showcell(51, T) << " | " << LudoBoard.Showcell(52, T) << " | " << LudoBoard.Showcell(3, T) << " |                       |" << endl;
    cout << "|                        |---|---|===|                       |" << endl;
    cout << "|                        | " << LudoBoard.Showcell(50, T) << " | " << LudoBoard.Showcell(53, T) << " | " << LudoBoard.Showcell(4, T) << " |                       |" << endl;
    cout << "|                        |---|---|---|                       |" << endl;
    cout << "|                        | " << LudoBoard.Showcell(49, T) << " | " << LudoBoard.Showcell(54, T) << " | " << LudoBoard.Showcell(5, T) << " |                       |" << endl;
    cout << "|                        |---|---|---|                       |" << endl;
    cout << "|                        | " << LudoBoard.Showcell(48, T) << " | " << LudoBoard.Showcell(55, T) << " | " << LudoBoard.Showcell(6, T) << " |                       |" << endl;
    cout << "|                        |---|---|---|                       |" << endl;
    cout << "|                        | " << LudoBoard.Showcell(47, T) << " | " << LudoBoard.Showcell(56, T) << " | " << LudoBoard.Showcell(7, T) << " |                       |" << endl;
    cout << "-----===------------------------------------------------------" << endl;
    cout << "| " << LudoBoard.Showcell(41, T) << " | " << LudoBoard.Showcell(42, T) << " | " << LudoBoard.Showcell(43, T) << " | " << LudoBoard.Showcell(44, T) << " | " << LudoBoard.Showcell(45, T) << " | " << LudoBoard.Showcell(46, T) << " | " << "   LUDO    " << "| " << LudoBoard.Showcell(8, T) << " | " << LudoBoard.Showcell(9, T) << " | " << LudoBoard.Showcell(10, T) << " | " << LudoBoard.Showcell(11, T) << " | " << LudoBoard.Showcell(12, T) << " | " << LudoBoard.Showcell(13, T) << " | " << endl;
    cout << "-----===------------------            ------------------------" << endl;
    cout << "| " << LudoBoard.Showcell(40, T) << " | " << LudoBoard.Showcell(67, T) << " | " << LudoBoard.Showcell(68, T) << " | " << LudoBoard.Showcell(69, T) << " | " << LudoBoard.Showcell(70, T) << " | " << LudoBoard.Showcell(71, T) << " | " << "  PROJECT  " << "| " << LudoBoard.Showcell(61, T) << " | " << LudoBoard.Showcell(60, T) << " | " << LudoBoard.Showcell(59, T) << " | " << LudoBoard.Showcell(58, T) << " | " << LudoBoard.Showcell(57, T) << " | " << LudoBoard.Showcell(14, T) << " | " << endl;
    cout << "-------------------------            -----------------===-----" << endl;
    cout << "| " << LudoBoard.Showcell(39, T) << " | " << LudoBoard.Showcell(38, T) << " | " << LudoBoard.Showcell(37, T) << " | " << LudoBoard.Showcell(36, T) << " | " << LudoBoard.Showcell(35, T) << " | " << LudoBoard.Showcell(34, T) << " | " << "    DSA    " << "| " << LudoBoard.Showcell(20, T) << " | " << LudoBoard.Showcell(19, T) << " | " << LudoBoard.Showcell(18, T) << " | " << LudoBoard.Showcell(17, T) << " | " << LudoBoard.Showcell(16, T) << " | " << LudoBoard.Showcell(15, T) << " | " << endl;
    cout << "------------------------------------------------------===-----" << endl;
    cout << "|                        | " << LudoBoard.Showcell(33, T) << " | " << LudoBoard.Showcell(66, T) << " | " << LudoBoard.Showcell(21, T) << " |                       |" << endl;
    cout << "|                        |---|---|---|                       |" << endl;
    cout << "|                        | " << LudoBoard.Showcell(32, T) << " | " << LudoBoard.Showcell(65, T) << " | " << LudoBoard.Showcell(22, T) << " |                       |" << endl;
    cout << "|                        |---|---|---|                       |" << endl;
    cout << "|                        | " << LudoBoard.Showcell(31, T) << " | " << LudoBoard.Showcell(64, T) << " | " << LudoBoard.Showcell(23, T) << " |                       |" << endl;
    cout << "|                        |---|---|---|                       |" << endl;
    cout << "|                        | " << LudoBoard.Showcell(30, T) << " | " << LudoBoard.Showcell(63, T) << " | " << LudoBoard.Showcell(24, T) << " |                       |" << endl;
    cout << "|                        |===|---|---|                       |" << endl;
    cout << "|                        | " << LudoBoard.Showcell(29, T) << " | " << LudoBoard.Showcell(62, T) << " | " << LudoBoard.Showcell(25, T) << " |                       |" << endl;
    cout << "|                        |===|---|---|                       |" << endl;
    cout << "|                        | " << LudoBoard.Showcell(28, T) << " | " << LudoBoard.Showcell(27, T) << " | " << LudoBoard.Showcell(26, T) << " |                       |" << endl;
    cout << "--------------------------------------------------------------" << endl;
    cout << "User: ";
    User.Display();
    cout << "Computer: ";
    Com.Display();
}
void waittwoSecond()
{
    this_thread::sleep_for(chrono::seconds(2));
}

int rollDice()
{
    int roll = 0;
    cout << "Rolling Dice :)" << endl;
    waittwoSecond();
    srand(time(0));
    roll = (rand() % 6) + 1;
    cout << "Rolled: " << roll << endl;
    return roll;
}
int main()
{

    Stack Userpieces = Stack();
    Stack ComPieces = Stack();
    Player User = Player("Sameer");
    piece P = piece("G1");
    User.Addpiece(P);
    piece P2 = piece("G2");
    User.Addpiece(P2);
    piece P3 = piece("G3");
    User.Addpiece(P);
    piece P4 = piece("G4");
    User.Addpiece(P2);
    Userpieces.push(P);
    Userpieces.push(P2);
    Userpieces.push(P3);
    Userpieces.push(P4);

    Stack Usercompleted = Stack();
    Stack ComputerCompleted = Stack();

    Player Com = Player("Computer");
    piece C = piece("R1");
    Com.Addpiece(C);
    piece C2 = piece("R2");
    Com.Addpiece(C2);
    piece C3 = piece("R3");
    Com.Addpiece(C3);
    piece C4 = piece("R4");
    Com.Addpiece(C4);

    ComPieces.push(C4);
    ComPieces.push(C3);
    ComPieces.push(C2);
    ComPieces.push(C);

    bool userStart = false;
    bool ComStart = false;
    Board LudoBoard = Board();
    Turn T = COM;
    do
    {
        switch (T)
        {
        case COM:
        {
            cout << "---------------------------- COMPUTER TURN -----------------------" << endl;
            DisplayBoard(LudoBoard, T, Userpieces, ComPieces);
            int Number = rollDice();
            if (C.getpieceStatus() != INGAME && C2.getpieceStatus() != INGAME && C3.getpieceStatus() != INGAME && C4.getpieceStatus() != INGAME && Number != 6)
            {
                cout << "Cannot Open !" << endl;
            }
            else if (Number == 6 && C.getpieceStatus() != INGAME && C2.getpieceStatus() != INGAME && C3.getpieceStatus() != INGAME && C4.getpieceStatus() != INGAME)
            {
                int done;
                done = LudoBoard.Open(T, C, ComPieces);
                if (C.getpieceStatus() == INGAME)
                {
                }
                cout << C.getPieceindex() << endl;
                if (done == 0)
                {
                    cout << "Cannot Open!" << endl;
                }
                if (done == 1)
                {
                    cout << "Now you can play!" << endl;
                    ComStart = true;
                    T = USER;
                    continue;
                }
            }
            if (ComStart == true)
            {
                Tree Tr = Tree(C, C2, C3, C4);
                Tr.BuildTree(P, P2, P3, P4);
                Tr.Decision(C, C2, C3, C4, Number, LudoBoard);
                int choice = Tr.StartDFS();
                piece *SelectedPiece = nullptr;
                switch (choice)
                {
                case 1:
                {
                    SelectedPiece = &C;
                    break;
                }
                case 2:
                {
                    SelectedPiece = &C2;
                    break;
                }
                case 3:
                {
                    SelectedPiece = &C3;
                    break;
                }
                case 4:
                {
                    SelectedPiece = &C4;
                    break;
                }
                }
                int Next = SelectedPiece->getPieceindex() + Number;
                cell &NextCell = LudoBoard.getcell(Next);
                if (Number == 6)
                {
                    if (SelectedPiece->getpieceStatus() == OUTGAME)
                    {
                        LudoBoard.Open(T, *SelectedPiece, ComPieces);
                    }
                    if (SelectedPiece->getpieceStatus() == INGAME)
                    {
                        LudoBoard.StepAhead(*SelectedPiece, Number, T);
                    }
                }
                else if (NextCell.getStatus() == OCCUPIED)
                {
                    LudoBoard.killMove(*SelectedPiece, Number, Userpieces, T);
                }
                else
                {
                    LudoBoard.StepAhead(*SelectedPiece, Number, T);
                }
            }
            T = USER;
        }
        case USER:
        {
            cout << "---------------------------- USER TURN -----------------------" << endl;
            DisplayBoard(LudoBoard, T, Userpieces, ComPieces);
            int Number = rollDice();
            if (P.getpieceStatus() != INGAME && P2.getpieceStatus() != INGAME && P3.getpieceStatus() != INGAME && P4.getpieceStatus() != INGAME && Number != 6)
            {
                cout << "Cannot Open !" << endl;
            }
            else if (Number == 6 && P.getpieceStatus() != INGAME && P2.getpieceStatus() != INGAME && P3.getpieceStatus() != INGAME && P4.getpieceStatus() != INGAME)
            {
                int done;
                done = LudoBoard.Open(T, P4, Userpieces);
                if (done == 0)
                {
                    cout << "Cannot Open!" << endl;
                }
                if (done == 1)
                {
                    cout << "Now you can play!" << endl;
                    userStart = true;
                    T = COM;
                    continue;
                }
            }
            if (userStart == true)
            {
                cout << "Select your Piece to Move: ";
                int choice;
                cin >> choice;
                piece *SelectedPiece = nullptr;
                switch (choice)
                {
                case 1:
                {
                    SelectedPiece = &P;
                    break;
                }
                case 2:
                {
                    SelectedPiece = &P2;
                    break;
                }
                case 3:
                {
                    SelectedPiece = &P3;
                    break;
                }
                case 4:
                {
                    SelectedPiece = &P4;
                    break;
                }
                }
                int Next = SelectedPiece->getPieceindex() + Number;
                cell &NextCell = LudoBoard.getcell(Next);
                if (Number == 6)
                {
                    if (SelectedPiece->getpieceStatus() == OUTGAME)
                    {
                        LudoBoard.Open(T, *SelectedPiece, Userpieces);
                    }
                    if (SelectedPiece->getpieceStatus() == INGAME)
                    {
                        LudoBoard.StepAhead(*SelectedPiece, Number, T);
                    }
                }
                else if (NextCell.getStatus() == OCCUPIED)
                {
                    LudoBoard.killMove(*SelectedPiece, Number, ComPieces, T);
                }
                else
                {
                    LudoBoard.StepAhead(*SelectedPiece, Number, T);
                }
            }
            T = COM;
        }
        }

    } while (!Usercompleted.Full() || !ComputerCompleted.Full());
}
