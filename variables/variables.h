int maxTime = 3;
bool abortSearch = false;

bool endgame = false;
int movesChecked = 0;

struct ArrayHasher {
    std::size_t operator()(const std::array<int, 81> &a) const {
        std::size_t h = 0;

        for (auto e : a) {
            h ^= std::hash<int>{}(e)  + 0x9e3779b9 + (h << 6) + (h >> 2); 
        }
        return h;
    }
};

std::unordered_map<std::array<int, 81>, int, ArrayHasher> transpositionTable;
