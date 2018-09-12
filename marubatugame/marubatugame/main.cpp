#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <array>
#include <memory>
#include <map>

using namespace std;

//�萔-------------
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

//�^----------
namespace TYPE
{
	template <typename T>
	using LINE	= array <	T		, GAME::BOARD_SIZE>;
	using MAP	= array <LINE<int>	, GAME::BOARD_SIZE>;
}

//�N���X�F�ʒu----------
class CintPOS2
{
	public:
		int x, y;
};



//�N���X�F�{�[�h���p-----
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

//�{�[�h���\�b�h�Q-----------------------------
CBoard::CBoard()
{
	//�{�[�h�̍쐬
	int board[GAME::BOARD_SIZE][GAME::BOARD_SIZE]{ 0 };

	memcpy(&m_board, board, sizeof(board));
}

//ID����z��̗v�f�ԍ������߂�
bool CBoard::MakeNoPos(int id, CintPOS2& pos)
{
	//ID����z��̗v�f�ԍ������߂�
	pos.x = id % GAME::BOARD_SIZE;
	pos.y = id / GAME::BOARD_SIZE;

	//���߂��v�f�ԍ����z��̗̈�𒴂��Ă�����
	if (CheckIfAreaOutside(pos))
	{
		//pos�̒l��ERROR�ɂ��Afalse��Ԃ�
		pos.x = GAME::ERROR_DATA;
		pos.y = GAME::ERROR_DATA;
		return false;
	}
	return true;
}
//�v�f�ԍ�����{�[�h�������߂�
bool CBoard::BoardSearch(CintPOS2 pos, int& id)
{
	//�v�f�ԍ����̈�𒴂��Ă�����false��Ԃ�
	if (CheckIfAreaOutside(pos))
	{
		id = GAME::ERROR_DATA;
		return false;
	}
		
	id = m_board[pos.y][pos.x];
	return true;
}
//�z��̗̈�𒴂��Ă��Ȃ����ǂ���
//�߂�l bool :�����Ă�����@true
bool CBoard::CheckIfAreaOutside(CintPOS2 pos)
{
	if (pos.x >= GAME::BOARD_SIZE)  return true;
	if (pos.y >= GAME::BOARD_SIZE)  return true;
	if (pos.x < 0)					return true;
	if (pos.y < 0)					return true;
	return false;
}
//��(�ZorX)�̐ݒu
//�߂�l bool :���� true
bool CBoard::SetStone(CintPOS2 pos,int id)
{
	//�ʒu���̈�𒴂��Ă��Ȃ����
	if (!CheckIfAreaOutside(pos))
	{
		//���łɒu����Ă��Ȃ�������
		if (m_board[pos.y][pos.x] == STONE::NONE)
		{
			m_board[pos.y][pos.x] = id;
			return true;
		}
	}
	return false;
}
//�u����ꏊ�����݂��邩�ǂ������`�F�b�N����
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
//�N���A���Ă���`����s��
void CBoard::Draw()
{
	//ID�\���p
	int count = 0;
	system("cls");
	for (auto &element : m_board)
	{
		for (int &stone : element)
		{
			switch (stone)
			{
			case STONE::NONE	: COUNT_DRAW(count); break;
			case STONE::CIRCLE	: cout << "[�Z]";	 break;
			case STONE::ClOS	: cout << "[�~]";		 break;
				default:break;
			}
			count++;
		}
		cout << "\n";
	}
}

//---------------------------------------------

//�N���X�F�Q�[���V�X�e��----
class CGameSystem
{
	public:
		CGameSystem(CBoard*);
		bool Input(int stone);
		bool ShechGameSet();
	private:
		CBoard* m_board;//�{�[�h���(�|�C���^)
		int	m_winner_id;//�Q�[���I���t���O���񏟎҂�ID
};
//�Q�[���V�X�e�����\�b�h�Q---------------------
CGameSystem::CGameSystem(CBoard* board)
{
	m_board = board;
	m_winner_id = STONE::NONE;
}

//�v���C���[����̓��͂��󂯁A�ǖʂ�ύX����
//�߂�l bool	:�u���邩�ǂ����̏�Ԃ�Ԃ�
bool CGameSystem::Input(int stone)
{
	//�u���ꏊ(�v�f�ԍ�)
	CintPOS2 pos;
	//�ZorX���z�u���ꂽ���ǂ���
	bool is_outsaid{ false };

	//�u����ꏊ�����݂��Ă��Ȃ�������
	if (m_board->CheckInputPlace() == false)
		return false;

	//�u����ꏊ�����݂��Ă���̂Œu���܂ŉ�
	while (is_outsaid == false)
	{
		//���͂Ƀ~�X������ꍇ�̓��[�v����
		if (stone == STONE::ClOS) cout << "\n�~";
		if (stone == STONE::CIRCLE) cout << "\n�Z";

		//���͏��擾�p�ϐ�
		int input_num;
		cout << "��u���ʒu��ԍ��œ��͂��Ȃ����B\n";
		cin >> input_num;

		//���͂��ꂽID����v�f�ԍ������߂�
		is_outsaid = m_board->MakeNoPos(input_num, pos);
		//ID����v�f�ԍ������܂�����
		//�z�u���s��
		if (is_outsaid)
		{
			is_outsaid = m_board->SetStone(pos,stone);
		} 

		//�{�[�h�̃N���A�ƍĕ`��
		m_board->Draw();

	}
	return true;
}
//�Q�[�����I�����Ă��邩�ǂ���(�r���S���Ă��邩�ǂ���)
//�߂�l bool	:�Q�[�����I�����Ă���@true
bool CGameSystem::ShechGameSet()
{
	//�{�[�h���𒲂ׂ邽�߂̕ϐ�
	CintPOS2 pos{0,0};
	//�A��(�r���S)���̎���STONE���(id)
	int chain_id{STONE::NONE};
	//�A���J�E���g
	int continuous_count{0};
	//�c�ɂ�����Ă��Ȃ����𒲂ׂ�
	for (pos.x = 0;pos.x < GAME::BOARD_SIZE; pos.x++)
	{
		for (pos.y = 0;pos.y < GAME::BOARD_SIZE; pos.y++)
		{
			//STONE���擾�p
			int id;
			//�v�f�ԍ�����{�[�h�����擾
			if (!m_board->BoardSearch(pos, id))
				return false;
			
			//STONE���NONE(�����Ȃ�)�łȂ����
			if (id != STONE::NONE)
				if (pos.y == 0)//�A������ŏ��Ȃ�
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
		//�A�������݂��Ă�����
		if (continuous_count == GAME::BOARD_SIZE)
		{
			//�����҂�ID�������Atrue��Ԃ�
			m_winner_id = chain_id;
			return true;
		}
		//�A���J�E���g����������
		continuous_count = 0;
	}
	//�����Ɏg�����ϐ������ׂď�����
	pos.x = pos.y = chain_id = continuous_count = 0;
	//���ɂ�����Ă��Ȃ����𒲂ׂ�
	for (pos.y = 0; pos.y < GAME::BOARD_SIZE; pos.y++)
	{
		for (pos.x = 0; pos.x < GAME::BOARD_SIZE; pos.x++)
		{
			//STONE���擾�p
			int id;
			//�v�f�ԍ�����{�[�h�����擾
			if (!m_board->BoardSearch(pos, id))
				return false;

			//STONE���NONE(�����Ȃ�)�łȂ����
			if (id != STONE::NONE)
				if (pos.x == 0)//�A������ŏ��Ȃ�
				{
					chain_id = id; continuous_count++;
				}
				else
					if (chain_id == id)
						continuous_count++;
					else
						break;

		}
		//�A�������݂��Ă�����
		if (continuous_count == GAME::BOARD_SIZE)
		{
			//�����҂�ID�������Atrue��Ԃ�
			m_winner_id = chain_id;
			return true;
		}
		//�A���J�E���g����������
		continuous_count = 0;
	}
	//�����Ɏg�����ϐ������ׂď�����
	pos.x = pos.y = chain_id = continuous_count = 0;
	//����`�E���ɂ����Ă͈̔͂𔻒肷��
	for (pos.x = 0,pos.y = 0; pos.x < GAME::BOARD_SIZE && pos.y < GAME::BOARD_SIZE; pos.x++, pos.y++)
	{
		//STONE���擾�p
		int id;
		//�v�f�ԍ�����{�[�h�����擾
		if (!m_board->BoardSearch(pos, id))
			return false;

		//STONE���NONE(�����Ȃ�)�łȂ����
		if (id != STONE::NONE)
			if (pos.y == 0)//�A������ŏ��Ȃ�
			{
				chain_id = id; continuous_count++;
			}
			else
				if (chain_id == id)
					continuous_count++;
				else
					break;

		//�A�������݂��Ă�����
		if (continuous_count == GAME::BOARD_SIZE)
		{
			//�����҂�ID�������Atrue��Ԃ�
			m_winner_id = chain_id;
			return true;
		}
	}
	//�����Ɏg�����ϐ������ׂď�����
	pos.x = pos.y = chain_id = continuous_count = 0;
	//����`�E���ɂ����Ă͈̔͂𔻒肷��
	for (pos.x = GAME::BOARD_SIZE-1,pos.y = 0; pos.x >= 0 && pos.y < GAME::BOARD_SIZE; pos.x--, pos.y++)
	{
		//STONE���擾�p
		int id;
		//�v�f�ԍ�����{�[�h�����擾
		if (!m_board->BoardSearch(pos, id))
			return false;

		//STONE���NONE(�����Ȃ�)�łȂ����
		if (id != STONE::NONE)
			if (pos.y == 0)//�A������ŏ��Ȃ�
			{
				chain_id = id; continuous_count++;
			}
			else
				if (chain_id == id)
					continuous_count++;
				else
					break;
		//�A�������݂��Ă�����
		if (continuous_count == GAME::BOARD_SIZE)
		{
			//�����҂�ID�������Atrue��Ԃ�
			m_winner_id = chain_id;
			return true;
		}
	}
	return false;
}
//--------------------------------------------

//���C��------------------
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