LEVEL_SIZE = 10
ENTIRE_PEOPLE_COUNT_BY_LEVEL = [4135, 174, 299, 58, 34, 25, 23, 9, 4, 1]
INVITED_PEOPLE_COUNT_BY_LEVEL = [2447, 120, 217, 43, 24, 19, 17, 5, 4, 0]
ENTIRE_FAUCET_COUNT_BY_LEVEL = [141644, 31255, 73496, 14274, 10160, 7806, 5890, 3576, 1959, 498]
INVITED_FAUCET_COUNT_BY_LEVEL = [103175, 23654, 52403, 10637, 8371, 5075, 3666, 2478, 1959, 0]
THRESHOLD_BY_LEVEL = [0 10000, 22738, 180000, 340000, 500000, 682000, 1137000, 2274000, 3411000]

def calc_reduction_rate():
    for i in range(LEVEL_SIZE):
        pb_reward = THRESHOLD_BY_LEVEL[i]
        invited_user = INVITED_PEOPLE_COUNT_BY_LEVEL[i]
        not_invited_user = ENTIRE_PEOPLE_COUNT_BY_LEVEL[i] - INVITED_PEOPLE_COUNT_BY_LEVEL[i]
        print(pb_reward())

calc_reduction_rate()
