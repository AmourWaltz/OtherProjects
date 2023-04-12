from rank import winner
from player import Player
from dealer import Dealer
from action import Action
from strategy import Strategy


class Game:
    """
    Passed a setting object the game will initialize the game settings to begin playing poker.
    """
    def __init__(self):
        self.players = []
        player1 = Player(1, 50)
        player2 = Player(2, 50)
        self.players.append(player1)
        self.players.append(player2)

        self.pot = 0
        self.winner = None
        pass

    def __str__(self):
        return "\n\t[Pot : " + str(self.pot) + "]\n"

    def play_one_state(self, state_type):
        last_action = None
        player_turn = 0
        print("state type: " + str(state_type))

        # While not FOLD
        while not last_action == Action(4):
            # Switch player
            player = self.players[player_turn % 2]
            # 0 for pre-flop
            player.act(state_type, last_action)

            # No more turns left, so go to the next state deal the next community cards
            if player.last_action is None:
                break
                pass
            pass

            player.print_last_action()

            # Update the new last action
            last_action = player.last_action
            player.add_actions(last_action)
            print("last action: " + str(last_action))

            # Update the current stack according to the action
            if player.decrease_stack(last_action):
                self.pot += 1
                if last_action.index == 3:
                    self.pot += 1
                    pass
                pass
            pass

            # Handle 2 special cases here and Call -> Check and Raise -> Bet
            if player_turn == 0:
                if last_action.index == 1:
                    last_action.index = 2
                    pass
                elif last_action.index == 3:
                    last_action.index = 2
                    pass
                pass
            pass

            # Update the counter
            player_turn += 1
            if last_action == Action(4):
                # other player won
                self.winner = self.players[player_turn % 2]
                return False
            pass
        return True

    # A completed round game process
    def play_one_round(self):
        # Create new dealer for every round
        dealer = Dealer()
        # Clear the pot by setting it to small + big blinds
        self.pot = 3
        self.winner = None

        # Player 0 is always small blind and Player 1 is always big blind
        self.players[0].set_blind(0)
        self.players[1].set_blind(1)

        self.players[0].set_cards(dealer.deal_pockets())
        self.players[1].set_cards(dealer.deal_pockets())
        self.show_game_status()

        # Three steps, they are pre-flop, pre-turn, pre-river respectively
        if self.play_one_state(0):  # pre-flop
            dealer.deal_flop()
            self.show_game_status()
            if self.play_one_state(1):  # pre-turn
                self.show_game_status()
                dealer.deal_turn()
                if self.play_one_state(1):  # pre-river
                    dealer.deal_river()
                    pass
                pass
            pass
        pass

        # Find the winner of the round
        if self.winner is not None:
            self.winner.increase_stack(self.pot)
            pass
        else:
            for player in self.players:
                player.show_cards()
                pass
            pass
            winner_id = winner(self.players[0], self.players[1], dealer.communitycards)
            print("Debugging Test winner_id : " + str(winner_id))
            
            # Restore the original hands
            for loop_i in range(6, 1, -1):
                for player in self.players:
                    player.hands.pop(loop_i)
                    pass
                pass
            pass

            # Winner get the pot. If it's a tie, halve the pot respectively
            if winner_id is not None:
                for player in self.players:
                    if player.check_id(winner_id):
                        player.increase_stack(self.pot)
                        self.winner = player
                        break
                        pass
                    pass
                pass
            else:
                for player in self.players:
                    player.increase_stack(self.pot / 2)
                    pass
                pass
            pass

        self.show_game_status()
        pass

    def start(self):
        game_end = False
        round_cnt = 0

        while round_cnt < 1 and not game_end:
            print("\n----------------------------------\n")
            self.play_one_round()
            print("\n----------------------------------\n")

            # Before going into the next round, check game over condition.
            for player in self.players:
                if player.judge_lost():
                    game_end = True
                    pass
                pass
            pass
            round_cnt += 1
            pass
        pass

    # Show player status
    def show_game_status(self):
        self.players[0].get_info()
        self.players[1].get_info()
        pass


# invictus_Gaming = Game()
# invictus_Gaming.start()
invictus_Gaming = Strategy()
invictus_Gaming.cfr_process()

# # Test if equal
# from card import Card
# 
# player3 = Player(3, 50)
# player4 = Player(4, 50)
# card1 = []
# card2 = []
# community = []
# card1.append(Card("heart", 3))
# card1.append(Card("heart", 2))
# card2.append(Card("club", 3))
# card2.append(Card("club", 2))
# player3.set_cards(card1)
# player4.set_cards(card2)
# community.append(Card("spade", 4))
# community.append(Card("spade", 5))
# community.append(Card("spade", 6))
# community.append(Card("spade", 7))
# community.append(Card("spade", 8))
# 
# print(winner(player3, player4, community))
