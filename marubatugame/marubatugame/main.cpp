#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <array>
#include <memory>
#include <map>

using namespace std;

//定数-------------
namespace GAME
{
	constexpr int BOARD_SIZE	{ 3 };
	constexpr bool GAME_LOOP{ true };
	constexpr int ERROR_DATA{ 999 };
}

namespace STONE
{
	constexpr int NONE		{ 0 };
	constexpr int CIRCLE	{ 1 };
	constexpr int ClOS		{ 2 };
}

//型----------
namespace TYPE
{
	template <typename T>
	using LINE	= array <	T		, GAME::BOARD_SIZE>;
	using MAP	= array <LINE<int>	, GAME::BOARD_SIZE>;
}

//クラス：位置----------
class CintPOS2
{
	public:
		int x, y;
};



//クラス：ボード情報用-----
class CBoard
{
	public:
		CBoard();
		bool MakeNoPos(int id, CintPOS2& pos);
		bool BoardSearch(CintPOS2 pos, int& id);
		bool CheckIfAreaOutside(CintPOS2 pos);
		bool SetStone(CintPOS2 pos, int id);
		bool CheckInputPlace();
		void Draw();

	private:
		TYPE::MAP		m_board;
		inline void COUNT_DRAW(int count) { cout << "[" << setw(2) << count << "]"; }

};

//ボードメソッド群-----------------------------
CBoard::CBoard()
{
	//ボードの作成
	int board[GAME::BOARD_SIZE][GAME::BOARD_SIZE]{ 0 };

	memcpy(&m_board, board, sizeof(board));
}

//IDから配列の要素番号を求める
bool CBoard::MakeNoPos(int id, CintPOS2& pos)
{
	//IDから配列の要素番号を求める
	pos.x = id % GAME::BOARD_SIZE;
	pos.y = id / GAME::BOARD_SIZE;

	//求めた要素番号が配列の領域を超えていたら
	if (CheckIfAreaOutside(pos))
	{
		//posの値をERRORにし、falseを返す
		pos.x = GAME::ERROR_DATA;
		pos.y = GAME::ERROR_DATA;
		return false;
	}
	return true;
}
//要素番号からボード情報を求める
bool CBoard::BoardSearch(CintPOS2 pos, int& id)
{
	//要素番号が領域を超えていたらfalseを返す
	if (CheckIfAreaOutside(pos))
	{
		id = GAME::ERROR_DATA;
		return false;
	}
		
	id = m_board[pos.y][pos.x];
	return true;
}
//配列の領域を超えていないかどうか
//戻り値 bool :超えていたら　true
bool CBoard::CheckIfAreaOutside(CintPOS2 pos)
{
	if (pos.x >= GAME::BOARD_SIZE)  return true;
	if (pos.y >= GAME::BOARD_SIZE)  return true;
	if (pos.x < 0)					return true;
	if (pos.y < 0)					return true;
	return false;
}
//石(〇orX)の設置
//戻り値 bool :成功 true
bool CBoard::SetStone(CintPOS2 pos,int id)
{
	//位置が領域を超えていなければ
	if (!CheckIfAreaOutside(pos))
	{
		//すでに置かれていなかったら
		if (m_board[pos.y][pos.x] == STONE::NONE)
		{
			m_board[pos.y][pos.x] = id;
			return true;
		}
	}
	return false;
}
//置ける場所が存在するかどうかをチェックする
bool CBoard::CheckInputPlace()
{
	for (auto &element : m_board)
	{
		for (int &stone : element)
		{
			if (stone == STONE::NONE)
				return true;
		}
	}
	return false;
}
//クリアしてから描画を行う
void CBoard::Draw()
{
	//ID表示用
	int count = 0;
	system("cls");
	for (auto &element : m_board)
	{
		for (int &stone : element)
		{
			switch (stone)
			{
			case STONE::NONE	: COUNT_DRAW(count); break;
			case STONE::CIRCLE	: cout << "[〇]";	 break;
			case STONE::ClOS	: cout << "[×]";		 break;
				default:break;
			}
			count++;
		}
		cout << "\n";
	}
}

//---------------------------------------------

//クラス：ゲームシステム----
class CGameSystem
{
	public:
		CGameSystem(CBoard*);
		bool Input(int stone);
		bool ShechGameSet();
	private:
		CBoard* m_board;//ボード情報(ポインタ)
		int	m_winner_id;//ゲーム終了フラグけん勝者のID
};
//ゲームシステムメソッド群---------------------
CGameSystem::CGameSystem(CBoard* board)
{
	m_board = board;
	m_winner_id = STONE::NONE;
}

//プレイヤーからの入力を受け、局面を変更する
//戻り値 bool	:置けるかどうかの状態を返す
bool CGameSystem::Input(int stone)
{
	//置く場所(要素番号)
	CintPOS2 pos;
	//〇orXが配置されたかどうか
	bool is_outsaid{ false };

	//置ける場所が存在していなかったら
	if (m_board->CheckInputPlace() == false)
		return false;

	//置ける場所が存在しているので置くまで回す
	while (is_outsaid == false)
	{
		//入力にミスがある場合はループする
		if (stone == STONE::ClOS) cout << "\n×";
		if (stone == STONE::CIRCLE) cout << "\n〇";

		//入力情報取得用変数
		int input_num;
		cout << "を置く位置を番号で入力しなさい。\n";
		cin >> input_num;

		//入力されたIDから要素番号を求める
		is_outsaid = m_board->MakeNoPos(input_num, pos);
		//IDから要素番号が求まったら
		//配置を行う
		if (is_outsaid)
		{
			is_outsaid = m_board->SetStone(pos,stone);
		} 

		//ボードのクリアと再描画
		m_board->Draw();

	}
	return true;
}
//ゲームが終了しているかどうか(ビンゴしているかどうか)
//戻り値 bool	:ゲームが終了している　true
bool CGameSystem::ShechGameSet()
{
	//ボード情報を調べるための変数
	CintPOS2 pos{0,0};
	//連鎖(ビンゴ)中の時のSTONE情報(id)
	int chain_id{STONE::NONE};
	//連続カウント
	int continuous_count{0};
	//縦にそろっていないかを調べる
	for (pos.x = 0;pos.x < GAME::BOARD_SIZE; pos.x++)
	{
		for (pos.y = 0;pos.y < GAME::BOARD_SIZE; pos.y++)
		{
			//STONE情報取得用
			int id;
			//要素番号からボード情報を取得
			if (!m_board->BoardSearch(pos, id))
				return false;
			
			//STONE情報がNONE(何もない)でなければ
			if (id != STONE::NONE)
				if (pos.y == 0)//連鎖判定最初なら
				{
					chain_id = id; continuous_count++;
				}
				else
					if (chain_id == id)
						continuous_count++;
					else
						break;
			else
				break;

		}
		//連鎖が存在していたら
		if (continuous_count == GAME::BOARD_SIZE)
		{
			//勝利者のIDを代入し、trueを返す
			m_winner_id = chain_id;
			return true;
		}
		//連続カウント数を初期化
		continuous_count = 0;
	}
	//処理に使った変数をすべて初期化
	pos.x = pos.y = chain_id = continuous_count = 0;
	//横にそろっていないかを調べる
	for (pos.y = 0; pos.y < GAME::BOARD_SIZE; pos.y++)
	{
		for (pos.x = 0; pos.x < GAME::BOARD_SIZE; pos.x++)
		{
			//STONE情報取得用
			int id;
			//要素番号からボード情報を取得
			if (!m_board->BoardSearch(pos, id))
				return false;

			//STONE情報がNONE(何もない)でなければ
			if (id != STONE::NONE)
				if (pos.x == 0)//連鎖判定最初なら
				{
					chain_id = id; continuous_count++;
				}
				else
					if (chain_id == id)
						continuous_count++;
					else
						break;

		}
		//連鎖が存在していたら
		if (continuous_count == GAME::BOARD_SIZE)
		{
			//勝利者のIDを代入し、trueを返す
			m_winner_id = chain_id;
			return true;
		}
		//連続カウント数を初期化
		continuous_count = 0;
	}
	//処理に使った変数をすべて初期化
	pos.x = pos.y = chain_id = continuous_count = 0;
	//左上～右下にかけての範囲を判定する
	for (pos.x = 0,pos.y = 0; pos.x < GAME::BOARD_SIZE && pos.y < GAME::BOARD_SIZE; pos.x++, pos.y++)
	{
		//STONE情報取得用
		int id;
		//要素番号からボード情報を取得
		if (!m_board->BoardSearch(pos, id))
			return false;

		//STONE情報がNONE(何もない)でなければ
		if (id != STONE::NONE)
			if (pos.y == 0)//連鎖判定最初なら
			{
				chain_id = id; continuous_count++;
			}
			else
				if (chain_id == id)
					continuous_count++;
				else
					break;

		//連鎖が存在していたら
		if (continuous_count == GAME::BOARD_SIZE)
		{
			//勝利者のIDを代入し、trueを返す
			m_winner_id = chain_id;
			return true;
		}
	}
	//処理に使った変数をすべて初期化
	pos.x = pos.y = chain_id = continuous_count = 0;
	//左上～右下にかけての範囲を判定する
	for (pos.x = GAME::BOARD_SIZE-1,pos.y = 0; pos.x >= 0 && pos.y < GAME::BOARD_SIZE; pos.x--, pos.y++)
	{
		//STONE情報取得用
		int id;
		//要素番号からボード情報を取得
		if (!m_board->BoardSearch(pos, id))
			return false;

		//STONE情報がNONE(何もない)でなければ
		if (id != STONE::NONE)
			if (pos.y == 0)//連鎖判定最初なら
			{
				chain_id = id; continuous_count++;
			}
			else
				if (chain_id == id)
					continuous_count++;
				else
					break;
		//連鎖が存在していたら
		if (continuous_count == GAME::BOARD_SIZE)
		{
			//勝利者のIDを代入し、trueを返す
			m_winner_id = chain_id;
			return true;
		}
	}
	return false;
}
//--------------------------------------------

//メイン------------------
int main()
{
	unique_ptr<CBoard> board(new CBoard());
	unique_ptr<CGameSystem> Game(new CGameSystem(board.get()));
	while (GAME::GAME_LOOP)
	{
		bool w, b;

		board->Draw();
		w = Game->Input(STONE::CIRCLE);
		if (Game->ShechGameSet())
			break;
		board->Draw();
		b = Game->Input(STONE::ClOS);
		if (Game->ShechGameSet())
			break;

		if (w == false && b == false) break;
	}
}