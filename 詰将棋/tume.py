UNKNOWN = 0

FRIEND = 1
ENEMY = 2

RESULT_UNKNOWN = 0
RESULT_TUMI = 1
RESULT_HUTUMI = 2

HU = 1 #歩
KY = 2 #香
KE = 3 #桂
GI = 4 #銀
KI = 5 #金
KA = 6 #角
HI = 7 #飛
OU = 8 #王
TO = 9 #と
NY = 10 #成香
NK = 11 #成桂
NG = 12 #成銀
UM = 13 #馬
RY = 14 #龍

INF = 100000

def display(display_board_num):

    RED = '\033[31m'
    BLUE = '\033[34m'
    YELLOW = '\033[33m'
    END = '\033[0m'

    display_board_string = []
    num = 0
    while(num < len(display_board_num)):
        if(display_board_num[num]==0):display_board_string.append("＊")
        elif(display_board_num[num]==1):display_board_string.append(BLUE+"歩"+END)
        elif(display_board_num[num]==2):display_board_string.append(BLUE+"香"+END)
        elif(display_board_num[num]==3):display_board_string.append(BLUE+"桂"+END)
        elif(display_board_num[num]==4):display_board_string.append(BLUE+"銀"+END)
        elif(display_board_num[num]==5):display_board_string.append(BLUE+"金"+END)
        elif(display_board_num[num]==6):display_board_string.append(BLUE+"角"+END)
        elif(display_board_num[num]==7):display_board_string.append(BLUE+"飛"+END)
        elif(display_board_num[num]==8):display_board_string.append(BLUE+"王"+END)
        elif(display_board_num[num]==9):display_board_string.append(BLUE+"と"+END)
        elif(display_board_num[num]==10):display_board_string.append(BLUE+"き"+END)
        elif(display_board_num[num]==11):display_board_string.append(BLUE+"け"+END)
        elif(display_board_num[num]==12):display_board_string.append(BLUE+"ぎ"+END)
        elif(display_board_num[num]==13):display_board_string.append(BLUE+"馬"+END)
        elif(display_board_num[num]==14):display_board_string.append(BLUE+"龍"+END)
        elif(display_board_num[num]==-1):display_board_string.append(RED+"歩"+END)
        elif(display_board_num[num]==-2):display_board_string.append(RED+"香"+END)
        elif(display_board_num[num]==-3):display_board_string.append(RED+"桂"+END)
        elif(display_board_num[num]==-4):display_board_string.append(RED+"銀"+END)
        elif(display_board_num[num]==-5):display_board_string.append(RED+"金"+END)
        elif(display_board_num[num]==-6):display_board_string.append(RED+"角"+END)
        elif(display_board_num[num]==-7):display_board_string.append(RED+"飛"+END)
        elif(display_board_num[num]==-8):display_board_string.append(RED+"王"+END)
        elif(display_board_num[num]==-9):display_board_string.append(RED+"と"+END)
        elif(display_board_num[num]==-10):display_board_string.append(RED+"き"+END)
        elif(display_board_num[num]==-11):display_board_string.append(RED+"け"+END)
        elif(display_board_num[num]==-12):display_board_string.append(RED+"ぎ"+END)
        elif(display_board_num[num]==-13):display_board_string.append(RED+"馬"+END)
        elif(display_board_num[num]==-14):display_board_string.append(RED+"龍"+END)
        elif(display_board_num[num]==101):display_board_string.append(YELLOW+"歩"+END)
        elif(display_board_num[num]==102):display_board_string.append(YELLOW+"香"+END)
        elif(display_board_num[num]==103):display_board_string.append(YELLOW+"桂"+END)
        elif(display_board_num[num]==104):display_board_string.append(YELLOW+"銀"+END)
        elif(display_board_num[num]==105):display_board_string.append(YELLOW+"金"+END)
        elif(display_board_num[num]==106):display_board_string.append(YELLOW+"角"+END)
        elif(display_board_num[num]==107):display_board_string.append(YELLOW+"飛"+END)
        elif(display_board_num[num]==108):display_board_string.append(YELLOW+"王"+END)
        elif(display_board_num[num]==109):display_board_string.append(YELLOW+"と"+END)
        elif(display_board_num[num]==110):display_board_string.append(YELLOW+"き"+END)
        elif(display_board_num[num]==111):display_board_string.append(YELLOW+"け"+END)
        elif(display_board_num[num]==112):display_board_string.append(YELLOW+"ぎ"+END)
        elif(display_board_num[num]==113):display_board_string.append(YELLOW+"馬"+END)
        elif(display_board_num[num]==114):display_board_string.append(YELLOW+"龍"+END)
        num+=1   
    print("""
    9  8  7  6  5  4  3  2  1
    %s %s %s %s %s %s %s %s %s一
    %s %s %s %s %s %s %s %s %s二
    %s %s %s %s %s %s %s %s %s三
    %s %s %s %s %s %s %s %s %s四
    %s %s %s %s %s %s %s %s %s五
    %s %s %s %s %s %s %s %s %s六
    %s %s %s %s %s %s %s %s %s七
    %s %s %s %s %s %s %s %s %s八
    %s %s %s %s %s %s %s %s %s九
    """ %tuple(display_board_string))

captured_list = [HU,KY,KE,GI,KI,KA,HI,OU]
captured_list_num = 8

promote_dict = {HU:TO,KY:NY,KE:NK,GI:NG,KA:UM,HI:RY}
reverse_promote_dict = {TO:HU,NY:KY,NK:KE,NG:GI,UM:KA,RY:HI}

class board_koma_status_class:
    def __init__(self,turn,koma_kind):
        self.turn = turn
        self.koma_kind = koma_kind

class move_status_class:
    def __init__(self,captured,turn,before_position,after_position,koma_kind):
        self.turn = turn
        self.koma_kind = koma_kind
        self.captured = captured
        self.before_position = before_position
        self.after_position = after_position

class state_class:
    def __init__(self,board,captured_friend,captured_enemy,Move):
        self.board = board
        self.captured_friend = captured_friend
        self.captured_enemy = captured_enemy
        self.Move = Move

class hash_class:
    def __init__(self,turn,pn,dn):
        self.turn = turn
        self.pn = pn
        self.dn = dn

class node_class:
    def __init__(self,result,state,children,parent,expanded,pn,dn,depth):
        self.result = result
        self.state = state
        self.children = children
        self.parent = parent
        self.expanded = expanded
        self.pn = pn
        self.dn = dn
        self.depth = depth


def serch_solved(node,path):
    if(node.result == RESULT_TUMI):
        path.append(node)
    len_children = len(node.children)
    num = 0
    while(num < len_children):
        serch_solved(node.children[num],path)
        num+=1


def change_x(xy):
    return (xy%9)

def change_y(xy):
    return int(xy/9)

def change_xy(x,y):
    if(x < 9 and y < 9 and x >=0 and y >=0):
        return y*9 + x
    else:
        return -100

def captured_add(koma_kind,captured):
    if(koma_kind in reverse_promote_dict):
        captured[reverse_promote_dict[koma_kind]]+=1
    else:
        captured[koma_kind]+=1

def captured_sub(koma_kind,captured):
    captured[koma_kind]-=1

def captured_sum(captured):
    sum = 0
    num = 0
    while(num < captured_list_num):
        sum += captured[captured_list[num]]
        num+=1
    return sum
        
def captured_available(captured):
    results = []
    num = 0
    while(num < captured_list_num):
        if(captured[captured_list[num]]!=0):
            results.append(captured_list[num])
        num+=1
    return results

def serch_target(board):
    y = 0
    while(y < 9):
        x = 0
        while(x < 9):
            koma = board[change_xy(x,y)]
            if(koma != 0 and koma.turn == ENEMY and koma.koma_kind == OU):
                return change_xy(x,y)
            x+=1
        y+=1
    return 0

def check_mate(turn,board):
    y = 0
    target_position = -1
    while(y < 9):
        x = 0
        while(x < 9):
            koma = board[change_xy(x,y)]
            if(koma != 0 and koma.koma_kind == OU and koma.turn != turn):
                target_position = change_xy(x,y)
                break
            x+=1
        if(target_position != -1):
            break
        y+=1
    moves = candidate_moves(board,turn)
    len_moves = len(moves)
    num = 0
    while(num < len_moves):
        if(moves[num].after_position == target_position):
            return moves[num]
        num+=1
    return 0

def move_koma(move,state):
    state.Move = move
    if(move.captured == True):
        state.board[move.after_position] = board_koma_status_class(move.turn,move.koma_kind)
        if(move.turn == FRIEND):
            captured_sub(move.koma_kind,state.captured_friend)
        elif(move.turn == ENEMY):
            captured_sub(move.koma_kind,state.captured_enemy)
    elif(move.captured == False):
        if(state.board[move.after_position] != 0):
            if(move.turn == FRIEND):
                captured_add(state.board[move.after_position].koma_kind,state.captured_friend)
            elif(move.turn == ENEMY):
                captured_add(state.board[move.after_position].koma_kind,state.captured_enemy)
        state.board[move.before_position] = 0
        state.board[move.after_position] = board_koma_status_class(move.turn,move.koma_kind)

#指定の駒の動きを返す。自分の駒を含んだら含まない、相手の駒を含んだら含む
def movable_positions(board,koma,koma_position):

    position = []
    koma_x = change_x(koma_position)
    koma_y = change_y(koma_position)

    if(koma.turn == FRIEND):
        if(koma.koma_kind == HU):
            position.append(change_xy(koma_x,koma_y-1))
        elif(koma.koma_kind == KY):
            for delta in range(1,9):
                if not(koma_x < 9 and koma_y-delta < 9 and koma_x >=0 and koma_y-delta >=0):
                    break
                position.append(change_xy(koma_x,koma_y-delta))
                if(board[change_xy(koma_x,koma_y-delta)] != 0):
                    break
        elif(koma.koma_kind == KE):
            position.append(change_xy(koma_x-1,koma_y-2))
            position.append(change_xy(koma_x+1,koma_y-2))
        elif(koma.koma_kind == GI):
            position.append(change_xy(koma_x,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y-1))
        elif(koma.koma_kind == KI):
            position.append(change_xy(koma_x,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y))
            position.append(change_xy(koma_x,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y))
            position.append(change_xy(koma_x-1,koma_y-1))
        elif(koma.koma_kind == KA):
            for delta in range(1,9):
                if not(koma_x-delta < 9 and koma_y-delta < 9 and koma_x-delta >=0 and koma_y-delta >=0):
                    break
                position.append(change_xy(koma_x-delta,koma_y-delta))
                if(board[change_xy(koma_x-delta,koma_y-delta)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x+delta < 9 and koma_y-delta < 9 and koma_x+delta >=0 and koma_y-delta >=0):
                    break
                position.append(change_xy(koma_x+delta,koma_y-delta))
                if(board[change_xy(koma_x+delta,koma_y-delta)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x+delta < 9 and koma_y+delta < 9 and koma_x+delta >=0 and koma_y+delta >=0):
                    break
                position.append(change_xy(koma_x+delta,koma_y+delta))
                if(board[change_xy(koma_x+delta,koma_y+delta)]):
                    break
            for delta in range(1,9):
                if not(koma_x-delta < 9 and koma_y+delta < 9 and koma_x-delta >=0 and koma_y+delta >=0):
                    break
                position.append(change_xy(koma_x-delta,koma_y+delta))
                if(board[change_xy(koma_x-delta,koma_y+delta)] != 0):
                    break
        elif(koma.koma_kind == HI):
            for delta in range(1,9):
                if not(koma_x < 9 and koma_y-delta < 9 and koma_x >=0 and koma_y-delta >=0):
                    break
                position.append(change_xy(koma_x,koma_y-delta))
                if(board[change_xy(koma_x,koma_y-delta)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x+delta < 9 and koma_y < 9 and koma_x+delta >=0 and koma_y >=0):
                    break
                position.append(change_xy(koma_x+delta,koma_y))
                if(board[change_xy(koma_x+delta,koma_y)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x < 9 and koma_y+delta < 9 and koma_x >=0 and koma_y+delta >=0):
                    break
                position.append(change_xy(koma_x,koma_y+delta))
                if(board[change_xy(koma_x,koma_y+delta)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x-delta < 9 and koma_y < 9 and koma_x-delta >=0 and koma_y >=0):
                    break
                position.append(change_xy(koma_x-delta,koma_y))
                if(board[change_xy(koma_x-delta,koma_y)] != 0):
                    break
        elif(koma.koma_kind == OU):
            position.append(change_xy(koma_x,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y))
            position.append(change_xy(koma_x+1,koma_y+1))
            position.append(change_xy(koma_x,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y))
            position.append(change_xy(koma_x-1,koma_y-1))
        elif(koma.koma_kind == TO):
            position.append(change_xy(koma_x,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y))
            position.append(change_xy(koma_x,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y))
            position.append(change_xy(koma_x-1,koma_y-1))
        elif(koma.koma_kind == NY):
            position.append(change_xy(koma_x,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y))
            position.append(change_xy(koma_x,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y))
            position.append(change_xy(koma_x-1,koma_y-1))
        elif(koma.koma_kind == NK):
            position.append(change_xy(koma_x,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y))
            position.append(change_xy(koma_x,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y))
            position.append(change_xy(koma_x-1,koma_y-1))
        elif(koma.koma_kind == NG):
            position.append(change_xy(koma_x,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y))
            position.append(change_xy(koma_x,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y))
            position.append(change_xy(koma_x-1,koma_y-1))
        elif(koma.koma_kind == UM):
            for delta in range(1,9):
                if not(koma_x-delta < 9 and koma_y-delta < 9 and koma_x-delta >=0 and koma_y-delta >=0):
                    break
                position.append(change_xy(koma_x-delta,koma_y-delta))
                if(board[change_xy(koma_x-delta,koma_y-delta)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x+delta < 9 and koma_y-delta < 9 and koma_x+delta >=0 and koma_y-delta >=0):
                    break
                position.append(change_xy(koma_x+delta,koma_y-delta))
                if(board[change_xy(koma_x+delta,koma_y-delta)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x+delta < 9 and koma_y+delta < 9 and koma_x+delta >=0 and koma_y+delta >=0):
                    break
                position.append(change_xy(koma_x+delta,koma_y+delta))
                if(board[change_xy(koma_x+delta,koma_y+delta)]):
                    break
            for delta in range(1,9):
                if not(koma_x-delta < 9 and koma_y+delta < 9 and koma_x-delta >=0 and koma_y+delta >=0):
                    break
                position.append(change_xy(koma_x-delta,koma_y+delta))
                if(board[change_xy(koma_x-delta,koma_y+delta)] != 0):
                    break
            position.append(change_xy(koma_x,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y))
            position.append(change_xy(koma_x,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y))
        elif(koma.koma_kind == RY):
            for delta in range(1,9):
                if not(koma_x < 9 and koma_y-delta < 9 and koma_x >=0 and koma_y-delta >=0):
                    break
                position.append(change_xy(koma_x,koma_y-delta))
                if(board[change_xy(koma_x,koma_y-delta)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x+delta < 9 and koma_y < 9 and koma_x+delta >=0 and koma_y >=0):
                    break
                position.append(change_xy(koma_x+delta,koma_y))
                if(board[change_xy(koma_x+delta,koma_y)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x < 9 and koma_y+delta < 9 and koma_x >=0 and koma_y+delta >=0):
                    break
                position.append(change_xy(koma_x,koma_y+delta))
                if(board[change_xy(koma_x,koma_y+delta)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x-delta < 9 and koma_y < 9 and koma_x-delta >=0 and koma_y >=0):
                    break
                position.append(change_xy(koma_x-delta,koma_y))
                if(board[change_xy(koma_x-delta,koma_y)] != 0):
                    break
            position.append(change_xy(koma_x+1,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y-1))
    elif(koma.turn == ENEMY):
        if(koma.koma_kind == HU):
            position.append(change_xy(koma_x,koma_y+1))
        elif(koma.koma_kind == KY):
            for delta in range(1,9):
                if not(koma_x < 9 and koma_y+delta < 9 and koma_x >=0 and koma_y+delta >=0):
                    break
                position.append(change_xy(koma_x,koma_y+delta))
                if(board[change_xy(koma_x,koma_y+delta)] != 0):
                    break
        elif(koma.koma_kind == KE):
            position.append(change_xy(koma_x-1,koma_y+2))
            position.append(change_xy(koma_x+1,koma_y+2))
        elif(koma.koma_kind == GI):
            position.append(change_xy(koma_x+1,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y+1))
            position.append(change_xy(koma_x,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y-1))
        elif(koma.koma_kind == KI):
            position.append(change_xy(koma_x,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y))
            position.append(change_xy(koma_x+1,koma_y+1))
            position.append(change_xy(koma_x,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y))
        elif(koma.koma_kind == KA):
            for delta in range(1,9):
                if not(koma_x-delta < 9 and koma_y-delta < 9 and koma_x-delta >=0 and koma_y-delta >=0):
                    break
                position.append(change_xy(koma_x-delta,koma_y-delta))
                if(board[change_xy(koma_x-delta,koma_y-delta)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x+delta < 9 and koma_y-delta < 9 and koma_x+delta >=0 and koma_y-delta >=0):
                    break
                position.append(change_xy(koma_x+delta,koma_y-delta))
                if(board[change_xy(koma_x+delta,koma_y-delta)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x+delta < 9 and koma_y+delta < 9 and koma_x+delta >=0 and koma_y+delta >=0):
                    break
                position.append(change_xy(koma_x+delta,koma_y+delta))
                if(board[change_xy(koma_x+delta,koma_y+delta)]):
                    break
            for delta in range(1,9):
                if not(koma_x-delta < 9 and koma_y+delta < 9 and koma_x-delta >=0 and koma_y+delta >=0):
                    break
                position.append(change_xy(koma_x-delta,koma_y+delta))
                if(board[change_xy(koma_x-delta,koma_y+delta)] != 0):
                    break
        elif(koma.koma_kind == HI):
            for delta in range(1,9):
                if not(koma_x < 9 and koma_y-delta < 9 and koma_x >=0 and koma_y-delta >=0):
                    break
                position.append(change_xy(koma_x,koma_y-delta))
                if(board[change_xy(koma_x,koma_y-delta)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x+delta < 9 and koma_y < 9 and koma_x+delta >=0 and koma_y >=0):
                    break
                position.append(change_xy(koma_x+delta,koma_y))
                if(board[change_xy(koma_x+delta,koma_y)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x < 9 and koma_y+delta < 9 and koma_x >=0 and koma_y+delta >=0):
                    break
                position.append(change_xy(koma_x,koma_y+delta))
                if(board[change_xy(koma_x,koma_y+delta)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x-delta < 9 and koma_y < 9 and koma_x-delta >=0 and koma_y >=0):
                    break
                position.append(change_xy(koma_x-delta,koma_y))
                if(board[change_xy(koma_x-delta,koma_y)] != 0):
                    break
        elif(koma.koma_kind == OU):
            position.append(change_xy(koma_x,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y))
            position.append(change_xy(koma_x+1,koma_y+1))
            position.append(change_xy(koma_x,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y))
            position.append(change_xy(koma_x-1,koma_y-1))
        elif(koma.koma_kind == TO):
            position.append(change_xy(koma_x,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y))
            position.append(change_xy(koma_x+1,koma_y+1))
            position.append(change_xy(koma_x,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y))
        elif(koma.koma_kind == NY):
            position.append(change_xy(koma_x,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y))
            position.append(change_xy(koma_x+1,koma_y+1))
            position.append(change_xy(koma_x,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y))
        elif(koma.koma_kind == NK):
            position.append(change_xy(koma_x,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y))
            position.append(change_xy(koma_x+1,koma_y+1))
            position.append(change_xy(koma_x,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y))
        elif(koma.koma_kind == NG):
            position.append(change_xy(koma_x,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y))
            position.append(change_xy(koma_x+1,koma_y+1))
            position.append(change_xy(koma_x,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y))
        elif(koma.koma_kind == UM):
            for delta in range(1,9):
                if not(koma_x-delta < 9 and koma_y-delta < 9 and koma_x-delta >=0 and koma_y-delta >=0):
                    break
                position.append(change_xy(koma_x-delta,koma_y-delta))
                if(board[change_xy(koma_x-delta,koma_y-delta)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x+delta < 9 and koma_y-delta < 9 and koma_x+delta >=0 and koma_y-delta >=0):
                    break
                position.append(change_xy(koma_x+delta,koma_y-delta))
                if(board[change_xy(koma_x+delta,koma_y-delta)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x+delta < 9 and koma_y+delta < 9 and koma_x+delta >=0 and koma_y+delta >=0):
                    break
                position.append(change_xy(koma_x+delta,koma_y+delta))
                if(board[change_xy(koma_x+delta,koma_y+delta)]):
                    break
            for delta in range(1,9):
                if not(koma_x-delta < 9 and koma_y+delta < 9 and koma_x-delta >=0 and koma_y+delta >=0):
                    break
                position.append(change_xy(koma_x-delta,koma_y+delta))
                if(board[change_xy(koma_x-delta,koma_y+delta)] != 0):
                    break
            position.append(change_xy(koma_x,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y))
            position.append(change_xy(koma_x,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y))
        elif(koma.koma_kind == RY):
            for delta in range(1,9):
                if not(koma_x < 9 and koma_y-delta < 9 and koma_x >=0 and koma_y-delta >=0):
                    break
                position.append(change_xy(koma_x,koma_y-delta))
                if(board[change_xy(koma_x,koma_y-delta)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x+delta < 9 and koma_y < 9 and koma_x+delta >=0 and koma_y >=0):
                    break
                position.append(change_xy(koma_x+delta,koma_y))
                if(board[change_xy(koma_x+delta,koma_y)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x < 9 and koma_y+delta < 9 and koma_x >=0 and koma_y+delta >=0):
                    break
                position.append(change_xy(koma_x,koma_y+delta))
                if(board[change_xy(koma_x,koma_y+delta)] != 0):
                    break
            for delta in range(1,9):
                if not(koma_x-delta < 9 and koma_y < 9 and koma_x-delta >=0 and koma_y >=0):
                    break
                position.append(change_xy(koma_x-delta,koma_y))
                if(board[change_xy(koma_x-delta,koma_y)] != 0):
                    break
            position.append(change_xy(koma_x+1,koma_y-1))
            position.append(change_xy(koma_x+1,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y+1))
            position.append(change_xy(koma_x-1,koma_y-1))

    result_positions = []

    num = 0
    len_position = len(position)
    while(len_position > num):
        if(change_x(position[num]) < 9 and change_y(position[num]) < 9 and change_x(position[num]) >=0 and change_y(position[num]) >=0):
            if(board[position[num]] != 0 and board[position[num]].turn == koma.turn):
                num+=1
                continue
            result_positions.append(position[num])
        num+=1

    return result_positions

#同じターンの盤面上の駒にmovable_positionsを適用する。成りも適用する。
def candidate_moves(board,turn):
    results = []
    y = 0
    while(y < 9):
        x = 0
        while(x < 9):
            before_koma_status = board[change_xy(x,y)]
            if(before_koma_status != 0 and before_koma_status.turn == turn):
                after_positions = movable_positions(board,before_koma_status,change_xy(x,y))
                num = 0
                len_after_positions = len(after_positions)
                while(num < len_after_positions):
                    must_promote = False
                    if(turn == FRIEND and change_y(after_positions[num]) <= 2):
                        if(before_koma_status.koma_kind == HU or before_koma_status.koma_kind == KY):
                            if(change_y(after_positions[num]) <= 0):
                                must_promote = True
                        elif(before_koma_status.koma_kind == KE):
                            if(change_y(after_positions[num]) <= 1):
                                must_promote = True
                        if(before_koma_status.koma_kind in promote_dict):
                            results.append(move_status_class(False,turn,change_xy(x,y),after_positions[num],promote_dict[before_koma_status.koma_kind]))
                    elif(turn == ENEMY and  change_y(after_positions[num])>= 6):
                        if(before_koma_status.koma_kind == HU or before_koma_status.koma_kind == KY):
                            if(change_y(after_positions[num]) >= 8):
                                must_promote = True
                        elif(before_koma_status.koma_kind == KE):
                            if(change_y(after_positions[num]) >= 7):
                                must_promote = True
                        if(before_koma_status.koma_kind in promote_dict):
                            results.append(move_status_class(False,turn,change_xy(x,y),after_positions[num],promote_dict[before_koma_status.koma_kind]))
                    if(must_promote == False):
                        results.append(move_status_class(False,turn,change_xy(x,y),after_positions[num],before_koma_status.koma_kind))
                    num+=1
            x+=1
        y+=1
    
    return results

def candidates(state,turn):
    results = []
    target = serch_target(state.board)
    
    moves = candidate_moves(state.board,turn)
    len_moves = len(moves)
    num = 0
    while(num < len_moves):
        new_state = state_class(state.board[:],state.captured_friend.(),state.captured_enemy.(),state.Move)
        move_koma(moves[num],new_state)
        check = check_mate(FRIEND,new_state.board)
        if((turn == FRIEND and check != 0) or (turn == ENEMY and check == 0)):
            results.append(new_state)
        num+=1
    if(turn == FRIEND):
        captured_putable_positions = []
        available_captureds = captured_available(state.captured_friend)
        len_available_captureds = len(available_captureds)
        num = 0
        while(num < len_available_captureds):
            if(available_captureds[num] == HU):
                captured_putable_positions.append(change_xy(0+change_x(target),1+change_y(target)))
            elif(available_captureds[num] == KY):
                y = 1
                while(change_y(target)+y < 9):
                    if(state.board[change_xy(change_x(target),change_y(target)+y)] != 0):
                        break
                    captured_putable_positions.append(change_xy(change_x(target),change_y(target)+y))
                    y+=1
            elif(available_captureds[num] == KE):
                captured_putable_positions.append(change_xy(1+change_x(target),2+change_y(target)))
                captured_putable_positions.append(change_xy(-1+change_x(target),2+change_y(target)))
            elif(available_captureds[num] == GI):
                captured_putable_positions.append(change_xy(-1+change_x(target),-1+change_y(target)))
                captured_putable_positions.append(change_xy(1+change_x(target),-1+change_y(target)))
                captured_putable_positions.append(change_xy(0+change_x(target),1+change_y(target)))
                captured_putable_positions.append(change_xy(-1+change_x(target),1+change_y(target)))
                captured_putable_positions.append(change_xy(1+change_x(target),1+change_y(target)))
            elif(available_captureds[num] == KI):
                captured_putable_positions.append(change_xy(0+change_x(target),-1+change_y(target)))
                captured_putable_positions.append(change_xy(-1+change_x(target),0+change_y(target)))
                captured_putable_positions.append(change_xy(1+change_x(target),0+change_y(target)))
                captured_putable_positions.append(change_xy(-1+change_x(target),1+change_y(target)))
                captured_putable_positions.append(change_xy(0+change_x(target),1+change_y(target)))
                captured_putable_positions.append(change_xy(1+change_x(target),1+change_y(target)))
            elif(available_captureds[num] == KA):
                direction_x = [-1,-1,1,1]
                direction_y =[-1,1,-1,1]
                num_direction = 0
                while(num_direction < 4):
                    xy = 1
                    while(True):
                        x = change_x(target) + direction_x[num_direction]*xy
                        y = change_y(target) + direction_y[num_direction]*xy
                        if(x < 9 and y < 9 and x >= 0 and y >= 0 and state.board[change_xy(x,y)] == 0):
                            captured_putable_positions.append(change_xy(x,y))
                        else:
                            break
                        xy+=1
                    num_direction+=1
            elif(available_captureds[num] == HI):
                direction_x = [-1,1,0,0]
                direction_y =[0,0,-1,1]
                num_direction = 0
                while(num_direction < 4):
                    xy = 1
                    while(True):
                        x = change_x(target) + direction_x[num_direction]*xy
                        y = change_y(target) + direction_y[num_direction]*xy
                        if(x < 9 and y < 9 and x >= 0 and y >= 0 and state.board[change_xy(x,y)] == 0):
                            captured_putable_positions.append(change_xy(x,y))
                        else:
                            break
                        xy+=1
                    num_direction+=1
            
            len_captured_putable_positions = len(captured_putable_positions)
            num_captured_putable_positions = 0
            while(num_captured_putable_positions < len_captured_putable_positions):
                x = change_x(captured_putable_positions[num_captured_putable_positions])
                y = change_y(captured_putable_positions[num_captured_putable_positions])
                if(x < 9 and y < 9 and x >= 0 and y >= 0 and state.board[change_xy(x,y)] == 0):
                    move = move_status_class(True,FRIEND,change_xy(9,9),captured_putable_positions[num_captured_putable_positions],available_captureds[num])
                    new_state = state_class(state.board[:],state.captured_friend.(),state.captured_enemy.(),state.Move)
                    move_koma(move,new_state)
                    results.append(new_state)
                num_captured_putable_positions+=1

            num+=1
    if(turn == ENEMY):
        if(captured_sum(state.captured_enemy) != 0):
            positions = []
            direction_x = [-1,-1,1,1,-1,1,0,0]
            direction_y =[-1,1,-1,1,0,0,-1,1]
            num = 0
            while(num < 8):
                positions_candidate = []
                xy = 1
                while(True):
                    x = change_x(target) + direction_x[num]*xy
                    y = change_y(target) + direction_y[num]*xy
                    if not(x >= 0 and y >= 0 and x < 9 and y < 9):
                        break
                    koma = state.board[change_xy(x,y)]
                    if(koma == 0):
                        positions_candidate.append(change_xy(x,y))
                    else:
                        if(direction_x[num]*direction_y[num] == 0 and koma.turn == FRIEND and ((koma.koma_kind == HI or koma.koma_kind == RY) or (koma.koma_kind == KY and direction_y[num] == 1)) ):
                            positions+=positions_candidate
                        if(direction_x[num]*direction_y[num] != 0 and koma.turn == FRIEND and (koma.koma_kind == KA or koma.koma_kind == UM)):
                            positions+=positions_candidate
                        break
                    xy+=1
                num+=1
            if(len(positions) > 0):
                available = captured_available(state.captured_enemy)
                for position in positions:
                    num = 0
                    while(num < len(available)):
                        if(available[num] == HU):
                            ok = True
                            y = 1
                            while(y < 9):
                                koma = state.board[change_xy(change_x(position),y)]
                                if(koma != 0 and koma.turn == ENEMY and koma.koma_kind == HU):
                                    ok = False
                                    break
                                y+=1
                            if(ok == False):
                                num+=1
                                continue
                        move = move_status_class(True,ENEMY,change_xy(9,9),position,available[num])
                        new_state = state_class(state.board[:],state.captured_friend.(),state.captured_enemy.(),state.Move)
                        move_koma(move,new_state)
                        check = check_mate(FRIEND,new_state.board)
                        if(check == 0):
                            results.append(new_state)
                        num+=1

    return results

def get_p(node):
    if(node.state.Move.turn == FRIEND):
        return node.dn
    elif(node.state.Move.turn == ENEMY):
        return node.pn

def set_p(node,num):
    if(node.state.Move.turn == FRIEND):
        node.dn = num
    elif(node.state.Move.turn == ENEMY):
        node.pn = num

def get_d(node):
    if(node.state.Move.turn == FRIEND):
        return node.pn
    elif(node.state.Move.turn == ENEMY):
        return node.dn

def set_d(node,num):
    if(node.state.Move.turn == FRIEND):
        node.pn = num
    elif(node.state.Move.turn == ENEMY):
        node.dn = num

def hash_pn(hash):
    if(hash.turn == FRIEND):
        return hash.dn
    elif(hash.turn == ENEMY):
        return hash.pn

def hash_dn(hash):
    if(hash.turn == FRIEND):
        return hash.pn
    elif(hash.turn == ENEMY):
        return hash.dn

def look_up_hash(node,hashs):
    if(node in hashs):
        return hashs[node]
    return hash_class(node.state.Move.turn,1,1)

def put_in_hash(node,pn,dn,hashs):
    hashs[node] = hash_class(node.state.Move.turn,pn,dn)
    
def min_delta(node,hashs):
    m = INF
    len_node_childrens = len(node.children)
    num = 0
    while(num < len_node_childrens):
        h = look_up_hash(node.children[num],hashs)
        d = hash_dn(h)
        if d < m:
            m = d
        num+=1
    return m

def sum_phi(node,hashs):
    s = 0
    len_node_childrens = len(node.children)
    num = 0
    while(num < len_node_childrens):
        h = look_up_hash(node.children[num],hashs)
        s+=hash_pn(h)
        num+=1
    return s

def select_child(node,hashs):
    children_second_dn = INF
    pn = INF
    dn = INF
    best = 0
    len_node_childrens = len(node.children)
    num = 0
    while(num < len_node_childrens):
        h = look_up_hash(node.children[num],hashs)
        if(hash_dn(h) < dn):
            best = num
            children_second_dn = dn
            pn = hash_pn(h)
            dn = hash_dn(h)
        elif(hash_dn(h) < children_second_dn):
            children_second_dn = hash_dn(h)
        if(hash_pn(h) == INF):
            return best,pn,dn,children_second_dn
        num+=1
    return best,pn,dn,children_second_dn


    


def solve(board,captured_friend,captured_enemy,max_depth):

    hashs = {}

    root = node_class(UNKNOWN,0,[],0,False,0,0,0)

    root.state = state_class(board,captured_friend,captured_enemy,0)
    root.state.Move = move_status_class(False,ENEMY,INF,INF,INF)
    root.pn = INF -1
    root.dn = INF -1

    mid(root,hashs,max_depth)
    if(root.pn < INF and root.dn < INF):
        root.pn = INF
        root.dn = INF
        mid(root,hashs,max_depth)
    
    if(root.pn > 0 and root.dn > 0):
        if(root.dn == 0):
            root.result = RESULT_TUMI
        if(root.pn == 0):
            root.result = RESULT_HUTUMI

    if(root.result == RESULT_UNKNOWN):
        root.result = RESULT_HUTUMI

    print(root.result)

    path = []
    max_depth_nodes = []

    serch_solved(root,path)
    len_path = len(path)
    num=0
    while(num < len_path):
        if(path[num].depth == max_depth):
            max_depth_nodes.append(path[num])
        num+=1

    for max_depth_node in max_depth_nodes:
        answers = []
        if(captured_sum(max_depth_node.state.captured_friend)==0):
            answers.append(max_depth_node)
            while(True):
                if(answers[-1].parent.depth == 0):
                    break
                else:
                    answers.append(answers[-1].parent)
            answers.reverse()
            for answer in answers:
                print("depth",answer.depth,"turn",answer.state.Move.turn,"koma_kind",answer.state.Move.koma_kind,"before_position",9-change_x(answer.state.Move.before_position),1+change_y(answer.state.Move.before_position),"after_position",9-change_x(answer.state.Move.after_position),1+change_y(answer.state.Move.after_position))
            print()


def mid(n,hashs,max_depth):
    
    h = look_up_hash(n,hashs)

    if(n.pn <= h.pn or n.dn <= h.dn):
        n.pn = h.pn
        n.dm = h.dn
        if(n.pn == 0):
            n.result = RESULT_TUMI
        if(n.dn == 0):
            n.result = RESULT_HUTUMI
        return

    if(n.expanded):
        if(len(n.children)==0):
            set_p(n,INF)
            set_d(n,0)
            put_in_hash(n,n.pn,n.dn,hashs)
            return
    else:
        cut_off = False
        if(max_depth != 0 and n.depth+1 > max_depth):
            if(n.state.Move.turn == ENEMY and n.state.Move.captured == False):
                cut_off = True
        if not(cut_off):
            if(n.state.Move.turn == FRIEND):
                candidate_array = candidates(n.state,ENEMY)
            elif(n.state.Move.turn == ENEMY):
                candidate_array = candidates(n.state,FRIEND)
            len_candidate_array = len(candidate_array)
            num = 0
            while(num < len_candidate_array):
                if(candidate_array[num].Move.turn == FRIEND and candidate_array[num].Move.captured == True and candidate_array[num].Move.koma_kind == HU):
                    if(len(candidates(candidate_array[num],ENEMY))==0):
                        num+=1
                        continue
                depth = n.depth+1
                if(n.state.Move.turn == ENEMY and n.state.Move.captured == True and n.state.Move.after_position == candidate_array[num].Move.after_position):
                    depth-=2

                n.children.append(node_class(UNKNOWN,candidate_array[num],[],n,False,0,0,depth))

                num+=1

        n.expanded = True
    put_in_hash(n,n.pn,n.dn,hashs)
    while(True):
        min_d = min_delta(n,hashs)
        sum_p = sum_phi(n,hashs)

        if(get_p(n) <= min_d or get_d(n) <= sum_p):
            set_p(n,min_d)
            set_d(n,sum_p)

            put_in_hash(n,n.pn,n.dn,hashs)

            if(n.pn == 0):
                n.result = RESULT_TUMI
            if(n.dn == 0):
                n.result = RESULT_HUTUMI

            return
        
        best,children_pn,children_dn,children_second_dn = select_child(n,hashs)

        best_children = n.children[best]

        if(children_pn == INF-1):
            set_p(best_children,INF)
        elif(get_d(n) >= INF-1):
            set_p(best_children,INF-1)
        else:
            set_p(best_children,(get_d(n) + children_pn - sum_p))
        
        if(children_dn == INF-1):
            set_d(best_children,INF)
        else:
            _dn = children_second_dn+1
            if(get_p(n) < _dn):
                _dn = get_p(n)
            set_d(best_children,_dn)
        
        mid(best_children,hashs,max_depth)



    
board_origin = [
    0,0,0,0,0,-RY,0,-OU,0,
    0,0,0,0,0,-HU,0,0,0,
    0,0,0,0,KY,0,-GI,0,0,
    0,0,0,0,0,0,RY,0,0,
    0,0,0,0,0,KA,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,
] 

captured_friend = {HU:0,KY:0,KE:0,GI:1,KI:0,KA:0,HI:0,OU:0}
captured_enemy = {HU:17,KY:3,KE:4,GI:2,KI:4,KA:1,HI:0,OU:0}   

board = []
for koma in board_origin:
    if(koma == 0):
        board.append(0)
    elif(koma > 0):
        board.append(board_koma_status_class(FRIEND,koma))
    elif(koma < 0):
        board.append(board_koma_status_class(ENEMY,-koma))
display(board_origin)
solve(board,captured_friend,captured_enemy,15)



