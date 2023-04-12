from action import Action, Action_Empty, Action_Set_1, Action_Set_2, Action_Set_3


class Player:
    """
    Player is a template class that has a get_strategy function which, in the
    case of a human player, is passed directly by player, and in the case of
    an AI player, the AI will calculate a decision using CFR

    The player also contains a number of attributes throughout the game and
    can yield those as a player_info object via get_info()
    """
    def __init__(self, number, stack):
        self.id = number
        self.stack = stack
        self.hands = []
        self.actions = []
        self.last_action = None
        self.is_huaman = True
        self.blind_type = 0

    def __str__(self):
        return "Player " + str(self.id) + " : " + str(self.stack)

    __repr__ = __str__

    # Return a set of player info for use in the game state
    def get_info(self):
        if self.hands is not None:
            return print(self.id, self.stack, self.hands, self.actions, self.is_huaman, self.blind_type)

    # Set the blind type, small for 1 or big for 2
    def set_blind(self, blind):
        self.blind_type = blind
        if blind == 0:
            self.stack -= 1
            pass
        elif blind == 1:
            self.stack -= 2
            pass
        pass

    # Get the cards in pocket
    def set_cards(self, cards):
        self.hands = cards
        pass

    # Print the cards in pocket
    def show_cards(self):
        print("Player " + str(self.id) + " : " + str(self.hands))
        pass

    # Judge the player whether
    def judge_lost(self):
        if self.stack <= 0:
            return True
        pass

    # Print the last action
    def print_last_action(self):
        string = "Player " + str(self.id)
        if self.last_action.index == 0:
            string += " checked."
            pass
        elif self.last_action.index == 1:
            string += " called."
            pass
        elif self.last_action.index == 2:
            string += " bet."
            pass
        elif self.last_action.index == 3:
            string += " raised."
            pass
        elif self.last_action.index == 4:
            string += " folded."
            pass
        print(string)

    # Check if the player has enough stack to raise or bet or call
    def act(self, state, each_action):
        actions = self.find_allowed_actions(state, each_action)
        if len(actions) == 0:
            self.last_action = None
            pass
        else:
            print(actions)
            if self.is_huaman is True:
                self.last_action = Action(int(input("Player " + str(self.id) + " : ")))
                pass
            # else:
            #     self.last_action = Action()
            # pass
        pass

    # Decrease the stack by action
    def decrease_stack(self, each_action):
        if each_action.index == 1 or each_action.index == 2:
            self.stack -= 1
            return True
        elif each_action.index == 3:
            self.stack -= 2
            return True
        return False

    def increase_stack(self, amount):
        self.stack += amount
        pass

    # Find possible actions given the state
    def find_allowed_actions(self, state, last_action):
        if last_action is None:
            # Pre-flop
            if state == 0:
                print("1")
                return Action_Set_1
            print("2")
            return Action_Set_2

        if self.blind_type == 0 and last_action.index == 0:
            print("3")
            return Action_Empty
        if last_action.index == 0:
            print("4")
            return Action_Set_2
        if last_action.index == 2:
            print("5")
            return Action_Set_1
        if last_action.index == 4:
            print("6")
            return Action_Empty

        if not state == 0 and last_action.index == 1:
            print("7")
            return Action_Empty
        if not state == 0 and last_action.index == 3:
            print("8")
            return Action_Set_3

        # For the pre-flop, if the last action was Call (1):
        # the possible action sets can be: S2, SE
        # Handle S2 case by changing that first Call to Check (0)
        if last_action.index == 1:
            print("9")
            return Action_Empty

        # For the pre-flop, if the last action was Raise (3):
        # the possible action sets can be: S1, S3
        # Handle S1 case by changing that first Raise to Bet (2)
        if last_action.index == 3:
            print("10")
            return Action_Set_3

    def add_actions(self, each_action):
        self.actions.append(each_action)
        pass

    def check_id(self, number):
        return self.id == number
