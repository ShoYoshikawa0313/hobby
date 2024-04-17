#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

using namespace std;

template <typename T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
	for (T const& i : v) {
		os << i << " ";
	}
	return os;
}

const int HU = 1;
const int KY = 2;
const int KE = 3;
const int GI = 4;
const int KI = 5;
const int KA = 6;
const int HI = 7;
const int OU = 8;
const int TO = 9;
const int NY = 10;
const int NK = 11;
const int NG = 12;
const int UM = 13;
const int RY = 14;

const bool TRUE = 1;
const bool FALSE = 0;

const int UNKNOWN = 0;
const int FRIEND = 1;
const int ENEMY = 2;

const int CAPTURED = -100;

const int RESULT_UNKNOWN = 0;
const int RESULT_TUMI = 1;
const int RESULT_HUTUMI = 2;

const int NO_HASH = -1;

const int INF = 100000000000000000;

struct Koma
{
	int kind;
	int turn;
};

struct Position
{
	int x;
	int y;
};

struct Move
{
	Position bef_pos;
	Position aft_pos;
	Koma koma;
};

struct State
{
	Koma board[9][9];
	int captured_friend[8];
	int captured_enemy[8];
};

struct Node
{
	int result;
	Move move;
	vector<unsigned int> children;
	unsigned int parent;
	bool expanded;
	unsigned int pn;
	unsigned int dn;
	unsigned int depth;
	unsigned int hash_num;
};

struct Hash
{
	int turn;
	unsigned int pn;
	unsigned int dn;
};

class solver
{
private:
	int captured_array[8] = { HU,KY,KE,GI,KI,KA,HI,OU };
	int len_captured_array = 8;
	map<int, int> promote_dict;
	map<int, int> reverse_promote_dict;

	unsigned int num_nodes = 0;
	unsigned int num_hashs = 0;

	queue<unsigned int> skip_nums_queue;

public:
	solver()
	{
		promote_dict[HU] = TO;
		promote_dict[KY] = NY;
		promote_dict[KE] = NK;
		promote_dict[GI] = NG;
		promote_dict[KA] = UM;
		promote_dict[HI] = RY;

		reverse_promote_dict[TO] = HU;
		reverse_promote_dict[NY] = KY;
		reverse_promote_dict[NK] = KE;
		reverse_promote_dict[NG] = GI;
		reverse_promote_dict[UM] = KA;
		reverse_promote_dict[RY] = HI;
	}

	void board_copy(Koma board[9][9], Koma new_board[9][9])
	{
		for (int y = 0; y < 9; y++)
		{
			for (int x = 0; x < 9; x++)
			{
				new_board[y][x] = board[y][x];
			}
		}
	}

	void captured_copy(int captured[8], int new_captured[8])
	{
		for (int num = 0; num < 8; num++)
		{
			new_captured[num] = captured[num];
		}
	}

	void state_copy(State* state, State* new_state)
	{
		board_copy((*state).board, (*new_state).board);
		captured_copy((*state).captured_friend, (*new_state).captured_friend);
		captured_copy((*state).captured_enemy, (*new_state).captured_enemy);
	}

	bool get_captured(Move* move)
	{
		if ((*move).bef_pos.x == CAPTURED && (*move).bef_pos.y == CAPTURED)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}

	void captured_add(int koma_kind, int captured[])
	{
		if (reverse_promote_dict.find(koma_kind) != reverse_promote_dict.end())
		{
			for (int num = 0; num < len_captured_array; num++)
			{
				if (captured_array[num] == reverse_promote_dict[koma_kind])
				{
					captured[num]++;
					break;
				}
			}
		}
		else
		{
			for (int num = 0; num < len_captured_array; num++)
			{
				if (captured_array[num] == koma_kind)
				{
					captured[num]++;
					break;
				}
			}
		}

	}

	void captured_sub(int koma_kind, int captured[])
	{
		for (int num = 0; num < len_captured_array; num++)
		{
			if (captured_array[num] == koma_kind)
			{
				captured[num]--;
				break;
			}
		}
	}

	void captured_available(int captured[], vector<int>* results)
	{
		for (int num = 0; num < 8; num++)
		{
			if (captured[num] != 0)
			{
				(*results).push_back(captured_array[num]);
			}
		}
	}

	int captured_sum(int captured[])
	{
		int sum = 0;
		for (int num = 0; num < 8; num++)
		{
			sum += captured[num];
		}
		return sum;
	}

	Position serch_target(Koma board[9][9], int target_turn)
	{
		for (int y = 0; y < 9; y++)
		{
			for (int x = 0; x < 9; x++)
			{
				Koma koma = board[y][x];
				if (koma.kind != 0 && koma.turn == target_turn && koma.kind == OU)
				{
					return { x,y };
				}
			}
		}
		//error
		return { -1,-1 };
	}

	void move_koma(Move* move, State* state)
	{
		if (get_captured(move) == TRUE)
		{
			((*state).board[(*move).aft_pos.y][(*move).aft_pos.x]) = (*move).koma;
			if ((*move).koma.turn == FRIEND)
			{
				captured_sub((*move).koma.kind, (*state).captured_friend);
			}
			else if ((*move).koma.turn == ENEMY)
			{
				captured_sub((*move).koma.kind, (*state).captured_enemy);
			}
		}
		else if (get_captured(move) == FALSE)
		{
			if ((*state).board[(*move).aft_pos.y][(*move).aft_pos.x].kind != 0)
			{
				if ((*move).koma.turn == FRIEND)
				{
					captured_add((*state).board[(*move).aft_pos.y][(*move).aft_pos.x].kind, (*state).captured_friend);
				}
				else if ((*move).koma.turn == ENEMY)
				{
					captured_add((*state).board[(*move).aft_pos.y][(*move).aft_pos.x].kind, (*state).captured_enemy);
				}
			}
			((*state).board[(*move).bef_pos.y][(*move).bef_pos.x]) = { 0,0 };
			((*state).board[(*move).aft_pos.y][(*move).aft_pos.x]) = (*move).koma;
		}
	}

	void move_board(vector<unsigned int>* path, State* new_state, vector<Node>* nodes)
	{
		for (int num = 0, len_path = (int)(*path).size(); num < len_path; num++)
		{
			move_koma(&(*nodes)[(*path)[num]].move, new_state);
		}
	}

	void movable_positions(Koma board[9][9], Koma koma, int x, int y, vector<Position>* movable_pos)
	{
		vector<Position> positions;

		if (koma.turn == FRIEND)
		{
			switch (koma.kind)
			{
			case HU:
			{
				positions.push_back({ x,y - 1 });
				break;
			}
			case KY:
			{
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x < 9 && y - delta < 9 && x >= 0 && y - delta >= 0)) { break; }
					positions.push_back({ x,y - delta });
					if (board[y - delta][x].kind != 0) { break; }
				}
				break;
			}
			case KE:
			{
				positions.push_back({ x - 1,y - 2 });
				positions.push_back({ x + 1,y - 2 });
				break;
			}
			case GI:
			{
				positions.push_back({ x,y - 1 });
				positions.push_back({ x + 1,y - 1 });
				positions.push_back({ x + 1,y + 1 });
				positions.push_back({ x - 1,y + 1 });
				positions.push_back({ x - 1,y - 1 });
				break;
			}
			case KI:
			{
				positions.push_back({ x,y - 1 });
				positions.push_back({ x + 1,y - 1 });
				positions.push_back({ x + 1,y });
				positions.push_back({ x,y + 1 });
				positions.push_back({ x - 1,y });
				positions.push_back({ x - 1,y - 1 });
				break;
			}
			case KA:
			{
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x - delta < 9 && y - delta < 9 && x - delta >= 0 && y - delta >= 0)) { break; }
					positions.push_back({ x - delta,y - delta });
					if (board[y - delta][x - delta].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x + delta < 9 && y - delta < 9 && x + delta >= 0 && y - delta >= 0)) { break; }
					positions.push_back({ x + delta,y - delta });
					if (board[y - delta][x + delta].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x + delta < 9 && y + delta < 9 && x + delta >= 0 && y + delta >= 0)) { break; }
					positions.push_back({ x + delta,y + delta });
					if (board[y + delta][x + delta].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x - delta < 9 && y + delta < 9 && x - delta >= 0 && y + delta >= 0)) { break; }
					positions.push_back({ x - delta,y + delta });
					if (board[y + delta][x - delta].kind != 0) { break; }
				}
				break;
			}
			case HI:
			{
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x < 9 && y - delta < 9 && x >= 0 && y - delta >= 0)) { break; }
					positions.push_back({ x ,y - delta });
					if (board[y - delta][x].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x + delta < 9 && y < 9 && x + delta >= 0 && y >= 0)) { break; }
					positions.push_back({ x + delta,y });
					if (board[y][x + delta].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x < 9 && y + delta < 9 && x >= 0 && y + delta >= 0)) { break; }
					positions.push_back({ x ,y + delta });
					if (board[y + delta][x].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x - delta < 9 && y < 9 && x - delta >= 0 && y >= 0)) { break; }
					positions.push_back({ x - delta,y });
					if (board[y][x - delta].kind != 0) { break; }
				}
				break;
			}
			case OU:
			{
				positions.push_back({ x ,y - 1 });
				positions.push_back({ x + 1,y - 1 });
				positions.push_back({ x + 1,y });
				positions.push_back({ x + 1,y + 1 });
				positions.push_back({ x ,y + 1 });
				positions.push_back({ x - 1 ,y + 1 });
				positions.push_back({ x - 1,y });
				positions.push_back({ x - 1,y - 1 });
				break;
			}
			case TO:
			{
				positions.push_back({ x,y - 1 });
				positions.push_back({ x + 1,y - 1 });
				positions.push_back({ x + 1,y });
				positions.push_back({ x,y + 1 });
				positions.push_back({ x - 1,y });
				positions.push_back({ x - 1,y - 1 });
				break;
			}
			case NY:
			{
				positions.push_back({ x,y - 1 });
				positions.push_back({ x + 1,y - 1 });
				positions.push_back({ x + 1,y });
				positions.push_back({ x,y + 1 });
				positions.push_back({ x - 1,y });
				positions.push_back({ x - 1,y - 1 });
				break;
			}
			case NK:
			{
				positions.push_back({ x,y - 1 });
				positions.push_back({ x + 1,y - 1 });
				positions.push_back({ x + 1,y });
				positions.push_back({ x,y + 1 });
				positions.push_back({ x - 1,y });
				positions.push_back({ x - 1,y - 1 });
				break;
			}
			case NG:
			{
				positions.push_back({ x,y - 1 });
				positions.push_back({ x + 1,y - 1 });
				positions.push_back({ x + 1,y });
				positions.push_back({ x,y + 1 });
				positions.push_back({ x - 1,y });
				positions.push_back({ x - 1,y - 1 });
				break;
			}
			case UM:
			{
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x - delta < 9 && y - delta < 9 && x - delta >= 0 && y - delta >= 0)) { break; }
					positions.push_back({ x - delta,y - delta });
					if (board[y - delta][x - delta].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x + delta < 9 && y - delta < 9 && x + delta >= 0 && y - delta >= 0)) { break; }
					positions.push_back({ x + delta,y - delta });
					if (board[y - delta][x + delta].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x + delta < 9 && y + delta < 9 && x + delta >= 0 && y + delta >= 0)) { break; }
					positions.push_back({ x + delta,y + delta });
					if (board[y + delta][x + delta].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x - delta < 9 && y + delta < 9 && x - delta >= 0 && y + delta >= 0)) { break; }
					positions.push_back({ x - delta,y + delta });
					if (board[y + delta][x - delta].kind != 0) { break; }
				}
				positions.push_back({ x ,y - 1 });
				positions.push_back({ x + 1,y });
				positions.push_back({ x ,y + 1 });
				positions.push_back({ x - 1,y });
				break;
			}
			case RY:
			{
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x < 9 && y - delta < 9 && x >= 0 && y - delta >= 0)) { break; }
					positions.push_back({ x ,y - delta });
					if (board[y - delta][x].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x + delta < 9 && y < 9 && x + delta >= 0 && y >= 0)) { break; }
					positions.push_back({ x + delta,y });
					if (board[y][x + delta].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x < 9 && y + delta < 9 && x >= 0 && y + delta >= 0)) { break; }
					positions.push_back({ x ,y + delta });
					if (board[y + delta][x].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x - delta < 9 && y < 9 && x - delta >= 0 && y >= 0)) { break; }
					positions.push_back({ x - delta,y });
					if (board[y][x - delta].kind != 0) { break; }
				}
				positions.push_back({ x + 1,y - 1 });
				positions.push_back({ x + 1,y + 1 });
				positions.push_back({ x - 1 ,y + 1 });
				positions.push_back({ x - 1,y - 1 });
				break;
			}





			}
		}
		else if (koma.turn == ENEMY)
		{
			switch (koma.kind)
			{
			case HU:
			{
				positions.push_back({ x,y + 1 });
				break;
			}
			case KY:
			{
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x < 9 && y + delta < 9 && x >= 0 && y + delta >= 0)) { break; }
					positions.push_back({ x,y + delta });
					if (board[y + delta][x].kind != 0) { break; }
				}
				break;
			}
			case KE:
			{
				positions.push_back({ x - 1,y + 2 });
				positions.push_back({ x + 1,y + 2 });
				break;
			}
			case GI:
			{
				positions.push_back({ x + 1,y - 1 });
				positions.push_back({ x + 1,y + 1 });
				positions.push_back({ x ,y + 1 });
				positions.push_back({ x - 1,y + 1 });
				positions.push_back({ x - 1,y - 1 });
				break;
			}
			case KI:
			{
				positions.push_back({ x,y - 1 });
				positions.push_back({ x + 1,y });
				positions.push_back({ x + 1,y + 1 });
				positions.push_back({ x,y + 1 });
				positions.push_back({ x - 1,y + 1 });
				positions.push_back({ x - 1,y });
				break;
			}
			case KA:
			{
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x - delta < 9 && y - delta < 9 && x - delta >= 0 && y - delta >= 0)) { break; }
					positions.push_back({ x - delta,y - delta });
					if (board[y - delta][x - delta].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x + delta < 9 && y - delta < 9 && x + delta >= 0 && y - delta >= 0)) { break; }
					positions.push_back({ x + delta,y - delta });
					if (board[y - delta][x + delta].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x + delta < 9 && y + delta < 9 && x + delta >= 0 && y + delta >= 0)) { break; }
					positions.push_back({ x + delta,y + delta });
					if (board[y + delta][x + delta].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x - delta < 9 && y + delta < 9 && x - delta >= 0 && y + delta >= 0)) { break; }
					positions.push_back({ x - delta,y + delta });
					if (board[y + delta][x - delta].kind != 0) { break; }
				}
				break;
			}
			case HI:
			{
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x < 9 && y - delta < 9 && x >= 0 && y - delta >= 0)) { break; }
					positions.push_back({ x ,y - delta });
					if (board[y - delta][x].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x + delta < 9 && y < 9 && x + delta >= 0 && y >= 0)) { break; }
					positions.push_back({ x + delta,y });
					if (board[y][x + delta].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x < 9 && y + delta < 9 && x >= 0 && y + delta >= 0)) { break; }
					positions.push_back({ x ,y + delta });
					if (board[y + delta][x].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x - delta < 9 && y < 9 && x - delta >= 0 && y >= 0)) { break; }
					positions.push_back({ x - delta,y });
					if (board[y][x - delta].kind != 0) { break; }
				}
				break;
			}
			case OU:
			{
				positions.push_back({ x ,y - 1 });
				positions.push_back({ x + 1,y - 1 });
				positions.push_back({ x + 1,y });
				positions.push_back({ x + 1,y + 1 });
				positions.push_back({ x ,y + 1 });
				positions.push_back({ x - 1 ,y + 1 });
				positions.push_back({ x - 1,y });
				positions.push_back({ x - 1,y - 1 });
				break;
			}
			case TO:
			{
				positions.push_back({ x,y - 1 });
				positions.push_back({ x + 1,y });
				positions.push_back({ x + 1,y + 1 });
				positions.push_back({ x,y + 1 });
				positions.push_back({ x - 1,y + 1 });
				positions.push_back({ x - 1,y });
				break;
			}
			case NY:
			{
				positions.push_back({ x,y - 1 });
				positions.push_back({ x + 1,y });
				positions.push_back({ x + 1,y + 1 });
				positions.push_back({ x,y + 1 });
				positions.push_back({ x - 1,y + 1 });
				positions.push_back({ x - 1,y });
				break;
			}
			case NK:
			{
				positions.push_back({ x,y - 1 });
				positions.push_back({ x + 1,y });
				positions.push_back({ x + 1,y + 1 });
				positions.push_back({ x,y + 1 });
				positions.push_back({ x - 1,y + 1 });
				positions.push_back({ x - 1,y });
				break;
			}
			case NG:
			{
				positions.push_back({ x,y - 1 });
				positions.push_back({ x + 1,y });
				positions.push_back({ x + 1,y + 1 });
				positions.push_back({ x,y + 1 });
				positions.push_back({ x - 1,y + 1 });
				positions.push_back({ x - 1,y });
				break;
			}
			case UM:
			{
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x - delta < 9 && y - delta < 9 && x - delta >= 0 && y - delta >= 0)) { break; }
					positions.push_back({ x - delta,y - delta });
					if (board[y - delta][x - delta].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x + delta < 9 && y - delta < 9 && x + delta >= 0 && y - delta >= 0)) { break; }
					positions.push_back({ x + delta,y - delta });
					if (board[y - delta][x + delta].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x + delta < 9 && y + delta < 9 && x + delta >= 0 && y + delta >= 0)) { break; }
					positions.push_back({ x + delta,y + delta });
					if (board[y + delta][x + delta].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x - delta < 9 && y + delta < 9 && x - delta >= 0 && y + delta >= 0)) { break; }
					positions.push_back({ x - delta,y + delta });
					if (board[y + delta][x - delta].kind != 0) { break; }
				}
				positions.push_back({ x ,y - 1 });
				positions.push_back({ x + 1,y });
				positions.push_back({ x ,y + 1 });
				positions.push_back({ x - 1,y });
				break;
			}
			case RY:
			{
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x < 9 && y - delta < 9 && x >= 0 && y - delta >= 0)) { break; }
					positions.push_back({ x ,y - delta });
					if (board[y - delta][x].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x + delta < 9 && y < 9 && x + delta >= 0 && y >= 0)) { break; }
					positions.push_back({ x + delta,y });
					if (board[y][x + delta].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x < 9 && y + delta < 9 && x >= 0 && y + delta >= 0)) { break; }
					positions.push_back({ x ,y + delta });
					if (board[y + delta][x].kind != 0) { break; }
				}
				for (int delta = 1; delta < 9; delta++)
				{
					if (!(x - delta < 9 && y < 9 && x - delta >= 0 && y >= 0)) { break; }
					positions.push_back({ x - delta,y });
					if (board[y][x - delta].kind != 0) { break; }
				}
				positions.push_back({ x + 1,y - 1 });
				positions.push_back({ x + 1,y + 1 });
				positions.push_back({ x - 1 ,y + 1 });
				positions.push_back({ x - 1,y - 1 });
				break;
			}

			}
		}

		for (int num = 0, len_positions = (int)positions.size(); num < len_positions; num++)
		{
			Position pos;
			pos = positions[num];
			if (pos.x < 9 && pos.y < 9 && pos.x >= 0 && pos.y >= 0)
			{
				if (board[pos.y][pos.x].kind != 0 && board[pos.y][pos.x].turn == koma.turn) { continue; }
				(*movable_pos).push_back({ pos.x,pos.y });
			}
		}

	}

	void candidate_moves(Koma board[9][9], int turn, vector<Move>* moves)
	{
		for (int bef_y = 0; bef_y < 9; bef_y++)
		{
			for (int bef_x = 0; bef_x < 9; bef_x++)
			{
				Koma bef_koma = board[bef_y][bef_x];
				if (bef_koma.kind != 0 && bef_koma.turn == turn)
				{
					vector<Position> after_positions;
					movable_positions(board, bef_koma, bef_x, bef_y, &after_positions);
					for (int num = 0, len_after_positions = (int)after_positions.size(); num < len_after_positions; num++)
					{
						Position aft_pos = after_positions[num];

						bool must_promote = FALSE;
						if (turn == FRIEND && (aft_pos.y <= 2 || bef_y <= 2))
						{
							if (bef_koma.kind == HU || bef_koma.kind == KY)
							{
								if (aft_pos.y <= 0)
								{
									must_promote = TRUE;
								}
							}
							else if (bef_koma.kind == KE)
							{
								if (aft_pos.y <= 1)
								{
									must_promote = TRUE;
								}
							}
							if (promote_dict.find(bef_koma.kind) != promote_dict.end())
							{
								(*moves).push_back({ {bef_x,bef_y},{aft_pos.x,aft_pos.y},{promote_dict[bef_koma.kind],turn} });
							}
						}
						else if (turn == ENEMY && (aft_pos.y >= 6 || bef_y >= 6))
						{
							if (bef_koma.kind == HU || bef_koma.kind == KY)
							{
								if (aft_pos.y >= 8)
								{
									must_promote = TRUE;
								}
							}
							else if (bef_koma.kind == KE)
							{
								if (aft_pos.y >= 7)
								{
									must_promote = TRUE;
								}
							}
							if (promote_dict.find(bef_koma.kind) != promote_dict.end())
							{
								(*moves).push_back({ {bef_x,bef_y},{aft_pos.x,aft_pos.y},{promote_dict[bef_koma.kind],turn} });
							}
						}
						if (must_promote == FALSE)
						{
							(*moves).push_back({ {bef_x,bef_y},{aft_pos.x,aft_pos.y},{bef_koma.kind,turn} });
						}
					}
				}
			}
		}
	}

	bool check_mate(int turn, Koma board[9][9])
	{
		Position target_position;
		if (turn == FRIEND)
		{
			target_position = serch_target(board, ENEMY);
		}
		else if (turn == ENEMY)
		{
			target_position = serch_target(board, FRIEND);
		}
		else
		{
			cout << "ERROR";
			exit(1);
		}

		vector<Move> moves;
		candidate_moves(board, turn, &moves);
		for (int num = 0, len_moves = (int)moves.size(); num < len_moves; num++)
		{
			if (moves[num].aft_pos.x == target_position.x && moves[num].aft_pos.y == target_position.y)
			{
				return TRUE;
			}
		}
		return FALSE;

	}

	void candidates(State* state, int turn, vector<Move>* results)
	{
		Position target_pos = serch_target((*state).board, ENEMY);
		vector<Move> moves;
		candidate_moves((*state).board, turn, &moves);

		for (int num = 0, len_moves = (int)moves.size(); num < len_moves; num++)
		{
			State new_state;
			state_copy(state, &new_state);
			move_koma(&moves[num], &new_state);
			bool check = check_mate(FRIEND, new_state.board);
			if ((turn == FRIEND && check == TRUE) || (turn == ENEMY && check == FALSE))
			{
				(*results).push_back(moves[num]);
			}
		}
		if (turn == FRIEND)
		{
			vector<int> available_captured;
			captured_available((*state).captured_friend, &available_captured);

			for (int num = 0, len_available_captured = (int)available_captured.size(); num < len_available_captured; num++)
			{
				vector<Position> positions;

				switch (available_captured[num])
				{
				case HU:
				{
					positions.push_back({ (0 + target_pos.x),(1 + target_pos.y) });
					break;
				}
				case KY:
				{
					for (int y = 1; target_pos.y + y < 9; y++)
					{
						if ((*state).board[target_pos.y + y][target_pos.x].kind != 0)
						{
							break;
						}
						positions.push_back({ target_pos.x,target_pos.y + y });
					}
					break;
				}
				case KE:
				{
					positions.push_back({ (1 + target_pos.x),(2 + target_pos.y) });
					positions.push_back({ (-1 + target_pos.x),(2 + target_pos.y) });
					break;
				}
				case GI:
				{
					positions.push_back({ (-1 + target_pos.x),(-1 + target_pos.y) });
					positions.push_back({ (1 + target_pos.x),(-1 + target_pos.y) });
					positions.push_back({ (0 + target_pos.x),(1 + target_pos.y) });
					positions.push_back({ (-1 + target_pos.x),(1 + target_pos.y) });
					positions.push_back({ (1 + target_pos.x),(1 + target_pos.y) });
					break;
				}
				case KI:
				{
					positions.push_back({ (0 + target_pos.x),(-1 + target_pos.y) });
					positions.push_back({ (-1 + target_pos.x),(0 + target_pos.y) });
					positions.push_back({ (1 + target_pos.x),(0 + target_pos.y) });
					positions.push_back({ (-1 + target_pos.x),(1 + target_pos.y) });
					positions.push_back({ (0 + target_pos.x),(1 + target_pos.y) });
					positions.push_back({ (1 + target_pos.x),(1 + target_pos.y) });
					break;
				}
				case KA:
				{
					int direction_x[4] = { -1,-1,1,1 };
					int direction_y[4] = { -1,1,-1,1 };
					for (int num_direction = 0; num_direction < 4; num_direction++)
					{
						int xy = 1;
						while (TRUE)
						{
							int x = target_pos.x + direction_x[num_direction] * xy;
							int y = target_pos.y + direction_y[num_direction] * xy;
							if (x < 9 && y < 9 && x >= 0 && y >= 0 && (*state).board[y][x].kind == 0)
							{
								positions.push_back({ x,y });
							}
							else
							{
								break;
							}
							xy++;
						}
					}
					break;
				}
				case HI:
				{
					int direction_x[4] = { -1,1,0,0 };
					int direction_y[4] = { 0,0,-1,1 };
					for (int num_direction = 0; num_direction < 4; num_direction++)
					{
						int xy = 1;
						while (TRUE)
						{
							int x = target_pos.x + direction_x[num_direction] * xy;
							int y = target_pos.y + direction_y[num_direction] * xy;
							if (x < 9 && y < 9 && x >= 0 && y >= 0 && (*state).board[y][x].kind == 0)
							{
								positions.push_back({ x,y });
							}
							else
							{
								break;
							}
							xy++;
						}
					}
					break;
				}
				}
				for (int num_positions = 0, len_positions = (int)positions.size(); num_positions < len_positions; num_positions++)
				{
					int x = positions[num_positions].x;
					int y = positions[num_positions].y;
					if (x < 9 && y < 9 && x >= 0 && y >= 0 && (*state).board[y][x].kind == 0)
					{
						(*results).push_back({ {CAPTURED,CAPTURED},{x,y},{available_captured[num],FRIEND} });
					}
				}
			}
		}
		else if (turn == ENEMY)
		{
			if (captured_sum((*state).captured_enemy) != 0)
			{
				vector<Position> positions;

				int hi_ry_direction_x[4] = { -1,1,0,0 };
				int hi_ry_direction_y[4] = { 0,0,-1,1 };
				for (int num = 0; num < 4; num++)
				{
					vector<Position> positions_candidate;
					int xy = 1;
					while (TRUE)
					{
						int x = target_pos.x + hi_ry_direction_x[num] * xy;
						int y = target_pos.y + hi_ry_direction_y[num] * xy;
						if (!(x >= 0 && y >= 0 && x < 9 && y < 9))
						{
							break;
						}
						Koma koma = (*state).board[y][x];
						if (koma.kind == 0)
						{
							positions_candidate.push_back({ x,y });
						}
						else
						{
							if (koma.turn == FRIEND && ((koma.kind == HI || koma.kind == RY) || (koma.kind == KY && hi_ry_direction_y[num] == 1)))
							{
								positions.insert(positions.end(), positions_candidate.begin(), positions_candidate.end());
							}
							break;
						}
						xy++;
					}
				}
				int ka_um_direction_x[4] = { -1,-1,1,1 };
				int ka_um_direction_y[4] = { -1,1,-1,1 };
				for (int num = 0; num < 4; num++)
				{
					vector<Position> positions_candidate;
					int xy = 1;
					while (TRUE)
					{
						int x = target_pos.x + ka_um_direction_x[num] * xy;
						int y = target_pos.y + ka_um_direction_y[num] * xy;
						if (!(x >= 0 && y >= 0 && x < 9 && y < 9))
						{
							break;
						}
						Koma koma = (*state).board[y][x];
						if (koma.kind == 0)
						{
							positions_candidate.push_back({ x,y });
						}
						else
						{
							if (koma.turn == FRIEND && (koma.kind == KA || koma.kind == UM))
							{
								positions.insert(positions.end(), positions_candidate.begin(), positions_candidate.end());
							}
							break;
						}
						xy++;
					}
				}

				if (positions.size() > 0)
				{
					vector<int> available;
					captured_available((*state).captured_enemy, &available);
					for (int pos_num = 0, len_positions = (int)positions.size(); pos_num < len_positions; pos_num++)
					{
						int pos_x = positions[pos_num].x;
						int pos_y = positions[pos_num].y;
						for (int num = 0, len_available = (int)available.size(); num < len_available; num++)
						{
							if (available[num] == HU)
							{
								bool nihu = FALSE;
								for (int y = 0; y < 9; y++)
								{
									Koma koma = (*state).board[y][pos_x];
									if (koma.kind != 0 && koma.turn == ENEMY && koma.kind == HU)
									{
										nihu = TRUE;
										break;
									}
								}
								if (nihu == TRUE)
								{
									continue;
								}
							}
							Move move = { {CAPTURED,CAPTURED},{pos_x,pos_y},{available[num],ENEMY} };
							State new_state;
							state_copy(state, &new_state);
							move_koma(&move, &new_state);
							bool check = check_mate(FRIEND, new_state.board);
							if (check == FALSE)
							{
								(*results).push_back(move);
							}
						}
					}
				}
			}
		}
	}

	unsigned int get_p(Node* node)
	{
		if ((*node).move.koma.turn == FRIEND)
		{
			return (*node).dn;
		}
		else if ((*node).move.koma.turn == ENEMY)
		{
			return (*node).pn;
		}
		else
		{
			cout << "ERROR";
			exit(1);
		}
	}

	void set_p(Node* node, unsigned int num)
	{
		if ((*node).move.koma.turn == FRIEND)
		{
			(*node).dn = num;
		}
		else if ((*node).move.koma.turn == ENEMY)
		{
			(*node).pn = num;
		}
	}

	unsigned int get_d(Node* node)
	{
		if ((*node).move.koma.turn == FRIEND)
		{
			return (*node).pn;
		}
		else if ((*node).move.koma.turn == ENEMY)
		{
			return (*node).dn;
		}
		else
		{
			cout << "ERROR";
			exit(1);
		}
	}

	void set_d(Node* node, unsigned int num)
	{
		if ((*node).move.koma.turn == FRIEND)
		{
			(*node).pn = num;
		}
		else if ((*node).move.koma.turn == ENEMY)
		{
			(*node).dn = num;
		}
	}

	unsigned int hash_pn(Hash* hash)
	{
		if ((*hash).turn == FRIEND)
		{
			return (*hash).dn;
		}
		else if ((*hash).turn == ENEMY)
		{
			return (*hash).pn;
		}
		else
		{
			cout << "ERROR";
			exit(1);
		}
	}

	unsigned int hash_dn(Hash* hash)
	{
		if ((*hash).turn == FRIEND)
		{
			return (*hash).pn;
		}
		else if ((*hash).turn == ENEMY)
		{
			return (*hash).dn;
		}
		else
		{
			cout << "ERROR";
			exit(1);
		}
	}

	Hash look_up_hash(Node* node, vector<Hash>* hashs)
	{
		if ((*node).hash_num != NO_HASH)
		{
			return (*hashs)[(*node).hash_num];
		}
		return { (*node).move.koma.turn,1,1 };
	}

	void put_in_hash(Node* node, vector<Hash>* hashs, unsigned int pn, unsigned int dn)
	{
		if ((*node).hash_num != NO_HASH)
		{
			(*hashs)[(*node).hash_num] = { (*node).move.koma.turn,pn,dn };
		}
		else
		{
			(*node).hash_num = num_hashs;
			(*hashs)[num_hashs++] = { (*node).move.koma.turn,pn,dn };
		}
	}

	unsigned int min_delta(unsigned int node_num, vector<Node>* nodes, vector<Hash>* hashs)
	{
		int unsigned m = INF;
		for (int num = 0, len_children = (int)(*nodes)[node_num].children.size(); num < len_children; num++)
		{
			Hash h = look_up_hash(&(*nodes)[(*nodes)[node_num].children[num]], hashs);
			unsigned int d = hash_dn(&h);
			if (d < m)
			{
				m = d;
			}
		}
		return m;
	}

	unsigned int sum_phi(unsigned int node_num, vector<Node>* nodes, vector<Hash>* hashs)
	{
		unsigned int s = 0;
		for (int num = 0, len_children = (int)(*nodes)[node_num].children.size(); num < len_children; num++)
		{
			Hash h = look_up_hash(&(*nodes)[(*nodes)[node_num].children[num]], hashs);
			s += hash_pn(&h);
		}
		return s;
	}

	void select_child(unsigned int node_num, vector<Node>* nodes, vector<Hash>* hashs, unsigned int* best, unsigned int* pn, unsigned int* dn, unsigned int* children_second_dn)
	{
		*children_second_dn = INF;
		*pn = INF;
		*dn = INF;
		*best = 0;
		for (int num = 0, len_children = (int)(*nodes)[node_num].children.size(); num < len_children; num++)
		{
			Hash h = look_up_hash(&(*nodes)[(*nodes)[node_num].children[num]], hashs);
			if (hash_dn(&h) < *dn)
			{
				*best = num;
				*children_second_dn = *dn;
				*pn = hash_pn(&h);
				*dn = hash_dn(&h);
			}
			else if (hash_dn(&h) < *children_second_dn)
			{
				*children_second_dn = hash_dn(&h);
			}
			if (hash_pn(&h) == INF)
			{
				break;
			}
		}
	}

	void serch_ancestors(unsigned int leaf_node_num, vector<Node>* nodes, vector<unsigned int>* ancestors)
	{
		(*ancestors).push_back(leaf_node_num);
		while (TRUE)
		{
			if ((*nodes)[(*ancestors)[(*ancestors).size() - 1]].parent == 0)
			{
				break;
			}
			else
			{
				(*ancestors).push_back((*nodes)[(*ancestors)[(*ancestors).size() - 1]].parent);
			}
		}
		reverse((*ancestors).begin(), (*ancestors).end());
	}

	void mid(unsigned int node_num, vector<Node>* nodes, vector<Hash>* hashs, unsigned int max_depth, State* initial_state)
	{
		Node* node = &((*nodes)[node_num]);
		State state;

		if ((*node).depth == 0)
		{
			state_copy(initial_state, &state);
		}
		else
		{
			vector<unsigned int> path;
			serch_ancestors(node_num, nodes, &path);
			state_copy(initial_state, &state);
			move_board(&path, &state, nodes);
		}

		Hash hash = look_up_hash(node, hashs);

		if ((*node).pn <= hash.pn || (*node).dn <= hash.dn)
		{
			(*node).pn = hash.pn;
			(*node).dn = hash.dn;
			if ((*node).pn == 0)
			{
				(*node).result = RESULT_TUMI;
			}
			if ((*node).dn == 0)
			{
				(*node).result = RESULT_HUTUMI;
			}
			return;
		}

		if ((*node).expanded)
		{
			if ((*node).children.size() == 0)
			{
				set_p(node, INF);
				set_d(node, 0);
				put_in_hash(node, hashs, (*node).pn, (*node).dn);
				return;
			}
		}
		else
		{
			bool cut_off = FALSE;
			if (max_depth != 0 && (*node).depth + 1 > max_depth)
			{
				if ((*node).move.koma.turn == ENEMY && get_captured(&(*node).move) == FALSE)
				{
					cut_off = TRUE;
				}
			}
			if (!cut_off)
			{
				vector<Move> candidate_array;
				if ((*node).move.koma.turn == FRIEND)
				{
					candidates(&state, ENEMY, &candidate_array);
				}
				else if ((*node).move.koma.turn == ENEMY)
				{
					candidates(&state, FRIEND, &candidate_array);
				}
				for (int num = 0, len_candidate_array = (int)candidate_array.size(); num < len_candidate_array; num++)
				{
					if (candidate_array[num].koma.turn == FRIEND && get_captured(&candidate_array[num]) == TRUE && candidate_array[num].koma.kind == HU)
					{
						State new_state;
						state_copy(&state, &new_state);
						move_koma(&candidate_array[num], &new_state);

						vector<Move> new_candidate_array;
						candidates(&new_state, ENEMY, &new_candidate_array);
						if (new_candidate_array.size() == 0)
						{
							continue;
						}
					}
					unsigned int depth = (*node).depth + 1;

					if ((*node).move.koma.turn == ENEMY && get_captured(&(*node).move) == TRUE && ((*node).move.aft_pos.x == candidate_array[num].aft_pos.x && (*node).move.aft_pos.y == candidate_array[num].aft_pos.y))
					{
						depth -= 2;
					}

					if (!skip_nums_queue.empty())
					{
						if ((int)skip_nums_queue.front() == num_nodes)
						{
							skip_nums_queue.pop();
							continue;
						}
					}

					vector<int> void_children;
					(*node).children.push_back(num_nodes);
					(*nodes)[num_nodes++] = { RESULT_UNKNOWN,candidate_array[num],void_children,node_num,FALSE,0,0,depth,NO_HASH };

				}
			}
			(*node).expanded = TRUE;
		}
		put_in_hash(node, hashs, (*node).pn, (*node).dn);
		while (TRUE)
		{
			unsigned int min_d = min_delta(node_num, nodes, hashs);
			unsigned int sum_p = sum_phi(node_num, nodes, hashs);

			if (get_p(node) <= min_d || get_d(node) <= sum_p)
			{
				set_p(node, min_d);
				set_d(node, sum_p);

				put_in_hash(node, hashs, (*node).pn, (*node).dn);

				if ((*node).pn == 0)
				{
					(*node).result = RESULT_TUMI;
				}
				if ((*node).dn == 0)
				{
					(*node).result = RESULT_HUTUMI;
				}
				return;
			}
			unsigned int best, children_pn, children_dn, children_second_dn;
			select_child(node_num, nodes, hashs, &best, &children_pn, &children_dn, &children_second_dn);

			unsigned int best_children_num = (*node).children[best];

			if (children_pn == INF - 1)
			{
				set_p(&(*nodes)[best_children_num], INF);
			}
			else if (get_d(node) >= INF - 1)
			{
				set_p(&(*nodes)[best_children_num], INF - 1);
			}
			else
			{
				set_p(&(*nodes)[best_children_num], (get_d(node) + children_pn - sum_p));
			}

			if (children_dn == INF - 1)
			{
				set_d(&(*nodes)[best_children_num], INF);
			}
			else
			{
				int _dn = children_second_dn + 1;
				if (get_p(node) < _dn)
				{
					_dn = get_p(node);
				}
				set_d(&(*nodes)[best_children_num], _dn);
			}

			mid(best_children_num, nodes, hashs, max_depth, initial_state);

		}
	}

	void diaplay_result(vector<unsigned int> *ancestors,vector<Node> *nodes, State *initial_state)
	{
		int bef_koma_x = 0;
		int bef_koma_y = 0;

		for(int num = 0,len_ancestors = (*ancestors).size();num < len_ancestors;num++)
		{	
			State bef_state;
			vector<unsigned int> bef_path;
			serch_ancestors((*ancestors)[num], nodes, &bef_path);
			state_copy(initial_state, &bef_state);
			move_board(&bef_path, &bef_state, nodes);

			State now_state;
			vector<unsigned int> now_path;
			serch_ancestors((*ancestors)[num], nodes, &now_path);
			state_copy(initial_state, &now_state);
			move_board(&now_path, &now_state, nodes);

			cout << (*nodes)[(*ancestors)[num]].depth;
			cout <<" ";
			if( (*nodes)[(*ancestors)[num]].move.koma.turn == FRIEND )
			{
				cout<<"▲";
			}
			else
			{
				cout<<"△";
			}
			if(bef_koma_x == (*nodes)[(*ancestors)[num]].move.aft_pos.x && bef_koma_y == (*nodes)[(*ancestors)[num]].move.aft_pos.y)
			{
				cout<<"同";
			}
			else
			{
				cout << (*nodes)[(*ancestors)[num]].move.aft_pos.x;
				cout << (*nodes)[(*ancestors)[num]].move.aft_pos.y;
			}

			bef_koma_x = (*nodes)[(*ancestors)[num]].move.aft_pos.x;
			bef_koma_y = (*nodes)[(*ancestors)[num]].move.aft_pos.y;

			int koma_kind = (*nodes)[(*ancestors)[num]].move.koma.kind;
			bool nari;
			if(!((*nodes)[(*ancestors)[num]].move.bef_pos.x == CAPTURED && (*nodes)[(*ancestors)[num]].move.bef_pos.y == CAPTURED))
			{
				if((*nodes)[(*ancestors)[num]].move.koma.kind != bef_state.board[(*nodes)[(*ancestors)[num]].move.bef_pos.y][(*nodes)[(*ancestors)[num]].move.bef_pos.x].kind )
				{
					nari = TRUE;
					koma_kind = reverse_promote_dict[ (*nodes)[(*ancestors)[num]].move.koma.kind ] ;
				}
			}
			switch(koma_kind)
			{
				case HU:
				{
					cout<<"歩";
					break;
				}
				case KY:
				{
					cout<<"香";
					break;
				}
				case KE:
				{
					cout<<"桂";
					break;
				}
				case GI:
				{
					cout<<"銀";
					break;
				}
				case KI:
				{
					cout<<"金";
					break;
				}
				case KA:
				{
					cout<<"角";
					break;
				}
				case HI:
				{
					cout<<"飛";
					break;
				}
				case OU:
				{
					cout<<"玉";
					break;
				}
				case TO:
				{
					cout<<"と";
					break;
				}
				case NY:
				{
					cout<<"成香";
					break;
				}
				case NK:
				{
					cout<<"成桂";
					break;
				}
				case NG:
				{
					cout<<"成銀";
					break;
				
				}
				case UM:
				{
					cout<<"馬";
					break;
				
				}
				case RY:
				{
					cout<<"龍";
					break;
				
				}

			}
			vector<Position> positions_candidate;
			Position target_pos = (*nodes)[(*ancestors)[num]].move.aft_pos;
			if((*nodes)[(*ancestors)[num]].move.koma.turn == FRIEND)
				switch(koma_kind)
				{
					case KE:
					{
						positions_candidate.push_back({ (1 + target_pos.x),(2 + target_pos.y) });
						positions_candidate.push_back({ (-1 + target_pos.x),(2 + target_pos.y) });
						break;
					}
					case GI:
					{
						positions_candidate.push_back({ (-1 + target_pos.x),(-1 + target_pos.y) });
						positions_candidate.push_back({ (1 + target_pos.x),(-1 + target_pos.y) });
						positions_candidate.push_back({ (0 + target_pos.x),(1 + target_pos.y) });
						positions_candidate.push_back({ (-1 + target_pos.x),(1 + target_pos.y) });
						positions_candidate.push_back({ (1 + target_pos.x),(1 + target_pos.y) });
						break;
					}
					case KI:
					case TO:
					case NY:
					case NK:
					case NG:
					{
						positions_candidate.push_back({ (0 + target_pos.x),(-1 + target_pos.y) });
						positions_candidate.push_back({ (-1 + target_pos.x),(0 + target_pos.y) });
						positions_candidate.push_back({ (1 + target_pos.x),(0 + target_pos.y) });
						positions_candidate.push_back({ (-1 + target_pos.x),(1 + target_pos.y) });
						positions_candidate.push_back({ (0 + target_pos.x),(1 + target_pos.y) });
						positions_candidate.push_back({ (1 + target_pos.x),(1 + target_pos.y) });
						break;
					}
					case UM:
					{
						positions_candidate.push_back({ (0 + target_pos.x),(-1 + target_pos.y) });
						positions_candidate.push_back({ (0 + target_pos.x),(1 + target_pos.y) });
						positions_candidate.push_back({ (-1 + target_pos.x),(0 + target_pos.y) });
						positions_candidate.push_back({ (1 + target_pos.x),(0 + target_pos.y) });
					}
					case KA:
					{
						int direction_x[4] = { -1,-1,1,1 };
						int direction_y[4] = { -1,1,-1,1 };
						for (int num_direction = 0; num_direction < 4; num_direction++)
						{
							int xy = 1;
							while (TRUE)
							{
								int x = target_pos.x + direction_x[num_direction] * xy;
								int y = target_pos.y + direction_y[num_direction] * xy;
								if ( !(x < 9 && y < 9 && x >= 0 && y >= 0) ) 
								{
									break;
								}
								else if ( now_state.board[y][x].kind == KA )
								{
									positions_candidate.push_back({ x,y });
								}
								else if ( now_state.board[y][x].kind != 0 )
								{
									break;
								}
								xy++;
							}
						}
						break;
					}
					case RY:
					{
						positions_candidate.push_back({ (1 + target_pos.x),(-1 + target_pos.y) });
						positions_candidate.push_back({ (1 + target_pos.x),(1 + target_pos.y) });
						positions_candidate.push_back({ (-1 + target_pos.x),(1 + target_pos.y) });
						positions_candidate.push_back({ (-1 + target_pos.x),(-1 + target_pos.y) });
					}
					case HI:
					{
						int direction_x[4] = { -1,1,0,0 };
						int direction_y[4] = { 0,0,-1,1 };
						for (int num_direction = 0; num_direction < 4; num_direction++)
						{
							int xy = 1;
							while (TRUE)
							{
								int x = target_pos.x + direction_x[num_direction] * xy;
								int y = target_pos.y + direction_y[num_direction] * xy;
								if ( !(x < 9 && y < 9 && x >= 0 && y >= 0) ) 
								{
									break;
								}
								else if ( now_state.board[y][x].kind == HI )
								{
									positions_candidate.push_back({ x,y });
								}
								else if ( now_state.board[y][x].kind != 0 )
								{
									break;
								}
								xy++;
							}
						}
						break;
					}
				}
			if((*nodes)[(*ancestors)[num]].move.koma.turn == ENEMY)
			{
				switch(koma_kind)
				{
					case KE:
					{
						positions_candidate.push_back({ (1 + target_pos.x),(-2 + target_pos.y) });
						positions_candidate.push_back({ (-1 + target_pos.x),(-2 + target_pos.y) });
						break;
					}
					case GI:
					{
						positions_candidate.push_back({ (0 + target_pos.x),(-1 + target_pos.y) });
						positions_candidate.push_back({ (1 + target_pos.x),(-1 + target_pos.y) });
						positions_candidate.push_back({ (1 + target_pos.x),(1 + target_pos.y) });
						positions_candidate.push_back({ (-1 + target_pos.x),(1 + target_pos.y) });
						positions_candidate.push_back({ (-1 + target_pos.x),(-1 + target_pos.y) });
						break;
					}
					case KI:
					case TO:
					case NY:
					case NK:
					case NG:
					{
						positions_candidate.push_back({ (0 + target_pos.x),(-1 + target_pos.y) });
						positions_candidate.push_back({ (1 + target_pos.x),(-1 + target_pos.y) });
						positions_candidate.push_back({ (1 + target_pos.x),(0 + target_pos.y) });
						positions_candidate.push_back({ (0 + target_pos.x),(1 + target_pos.y) });
						positions_candidate.push_back({ (-1 + target_pos.x),(0 + target_pos.y) });
						positions_candidate.push_back({ (-1 + target_pos.x),(-1 + target_pos.y) });
						break;
					}
					case UM:
					{
						positions_candidate.push_back({ (0 + target_pos.x),(-1 + target_pos.y) });
						positions_candidate.push_back({ (0 + target_pos.x),(1 + target_pos.y) });
						positions_candidate.push_back({ (-1 + target_pos.x),(0 + target_pos.y) });
						positions_candidate.push_back({ (1 + target_pos.x),(0 + target_pos.y) });
					}
					case KA:
					{
						int direction_x[4] = { -1,-1,1,1 };
						int direction_y[4] = { -1,1,-1,1 };
						for (int num_direction = 0; num_direction < 4; num_direction++)
						{
							int xy = 1;
							while (TRUE)
							{
								int x = target_pos.x + direction_x[num_direction] * xy;
								int y = target_pos.y + direction_y[num_direction] * xy;
								if ( !(x < 9 && y < 9 && x >= 0 && y >= 0) ) 
								{
									break;
								}
								else if ( now_state.board[y][x].kind == KA )
								{
									positions_candidate.push_back({ x,y });
								}
								else if ( now_state.board[y][x].kind != 0 )
								{
									break;
								}
								xy++;
							}
						}
						break;
					}
					case RY:
					{
						positions_candidate.push_back({ (1 + target_pos.x),(-1 + target_pos.y) });
						positions_candidate.push_back({ (1 + target_pos.x),(1 + target_pos.y) });
						positions_candidate.push_back({ (-1 + target_pos.x),(1 + target_pos.y) });
						positions_candidate.push_back({ (-1 + target_pos.x),(-1 + target_pos.y) });
					}
					case HI:
					{
						int direction_x[4] = { -1,1,0,0 };
						int direction_y[4] = { 0,0,-1,1 };
						for (int num_direction = 0; num_direction < 4; num_direction++)
						{
							int xy = 1;
							while (TRUE)
							{
								int x = target_pos.x + direction_x[num_direction] * xy;
								int y = target_pos.y + direction_y[num_direction] * xy;
								if ( !(x < 9 && y < 9 && x >= 0 && y >= 0) ) 
								{
									break;
								}
								else if ( now_state.board[y][x].kind == HI )
								{
									positions_candidate.push_back({ x,y });
								}
								else if ( now_state.board[y][x].kind != 0 )
								{
									break;
								}
								xy++;
							}
						}
						break;
					}
				}
			}
			vector<Position> positions;
			for(int N = 0,len_positions_candidate = positions_candidate.size();N < len_positions_candidate;N++)
			{
				if(now_state.board[positions_candidate[num].y][positions_candidate[num].x].kind == koma_kind && now_state.board[positions_candidate[num].y][positions_candidate[num].x].turn == (*nodes)[(*ancestors)[num]].move.koma.turn)
				{
					positions.push_back(positions_candidate[num]);
				}
			}
			if(positions.size() > 1)
			{
				
			}
		}
	}

	int solve(Koma board[9][9], int captured_friend[8], int captured_enemy[8], int max_depth, int max_node_num)
	{
		vector<Hash> hashs(max_node_num);
		vector<Node> nodes(max_node_num);

		State initial_state;
		board_copy(board, initial_state.board);
		captured_copy(captured_friend, initial_state.captured_friend);
		captured_copy(captured_enemy, initial_state.captured_enemy);

		vector<int> initial_children;
		nodes[num_nodes++] = { RESULT_UNKNOWN,{{-1,-1},{-1,-1},{-1,ENEMY}},initial_children,0,FALSE,INF - 1,INF - 1,0,NO_HASH };
		Node* root = &nodes[0];

		mid(0, &nodes, &hashs, max_depth, &initial_state);
		if ((*root).pn < INF && (*root).dn < INF)
		{
			(*root).pn = INF;
			(*root).dn = INF;
			mid(0, &nodes, &hashs, max_depth, &initial_state);
		}

		if ((*root).pn > 0 && (*root).dn > 0)
		{
			if ((*root).dn == 0)
			{
				(*root).result = RESULT_TUMI;
			}
			if ((*root).pn == 0)
			{
				(*root).result = RESULT_HUTUMI;
			}
		}
		if ((*root).result == RESULT_UNKNOWN)
		{
			(*root).result = RESULT_HUTUMI;
		}
		cout << num_nodes<<"\n";
		return (*root).result;
	}

};

int main(void)
{
	/*int E = ENEMY;
	int F = FRIEND;
	Koma board[9][9] = {
		{{HI,F},{RY,E},{0 ,0},{0 ,0},{0 ,0},{HU,E},{GI,F},{0 ,0},{0 ,0}},
		{{0 ,0},{KY,F},{KE,E},{0 ,0},{0 ,0},{KI,E},{0 ,0},{HU,E},{0 ,0}},
		{{0 ,0},{0 ,0},{0 ,0},{OU,E},{KE,E},{0 ,0},{0 ,0},{0 ,0},{KI,E}},
		{{0 ,0},{KA,F},{NY,E},{HU,E},{KY,E},{TO,F},{0 ,0},{KA,F},{KI,E}},
		{{0 ,0},{0 ,0},{0 ,0},{0 ,0},{0 ,0},{0 ,0},{0 ,0},{0 ,0},{KE,F}},
		{{TO,E},{0 ,0},{0 ,0},{0 ,0},{0 ,0},{0 ,0},{0 ,0},{0 ,0},{HU,E}},
		{{HU,F},{0 ,0},{HU,E},{HU,F},{0 ,0},{0 ,0},{GI,F},{0 ,0},{0 ,0}},
		{{TO,E},{0 ,0},{HU,F},{0 ,0},{0 ,0},{0 ,0},{0 ,0},{0 ,0},{0 ,0}},
		{{GI,F},{0 ,0},{0 ,0},{0 ,0},{0 ,0},{KE,F},{0 ,0},{KI,E},{0 ,0}},


	};

							  //HU,KY,KE,GI,KI,KA,HI,OU
	int capture_enemy[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int capture_friend[8] = { 5, 0, 0, 0, 0, 0, 0, 0 };
	solver game;
	cout << (int)game.solve(board, capture_friend, capture_enemy, 163, 1000000);*/
	
}
