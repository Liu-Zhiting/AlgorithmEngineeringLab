#include "utils.hpp"

uint32_t size = 0, length = 0, mod = 1;

ostringstream ostr;

int file_type_id, mode_id;
string mode, file_type;

map<string, void (*)(int32_t **)> func_map;

void save_text(int32_t **result);
void save_binary(int32_t **result);

void generate_random(int32_t **result);
void generate_sparse(int32_t **result);
void generate_eye(int32_t **result);
void generate_01(int32_t **result);

void init_func_map();
void parse_args(int argc, char **argv);

int main(int argc, char **argv)
{
    init_func_map();
    parse_args(argc, argv);

    for (int i = 0; i < length; i++)
        mod *= 10;

    ostr << "matrix_" << mode << '_' << size << '_' << length;

    int32_t **mat = new int32_t *[size];
    for (int i = 0; i < size; i++)
    {
        mat[i] = new int32_t[size];
        memset(mat[i], 0, size * sizeof(int32_t));
    }

    srand(time(NULL));

    func_map.find(mode)->second(mat);
    func_map.find(file_type)->second(mat);

    for (int i = 0; i < size; i++)
        delete[] mat[i];
    delete[] mat;
    mat = nullptr;
}

void init_func_map()
{
    func_map.insert(pair<string, void (*)(int32_t **)>("text", save_text));
    func_map.insert(pair<string, void (*)(int32_t **)>("binary", save_binary));
    func_map.insert(pair<string, void (*)(int32_t **)>("random", generate_random));
    func_map.insert(pair<string, void (*)(int32_t **)>("sparse", generate_sparse));
    func_map.insert(pair<string, void (*)(int32_t **)>("eye", generate_eye));
    func_map.insert(pair<string, void (*)(int32_t **)>("01", generate_01));
}

void parse_args(int argc, char **argv)
{
    if (argc < 5)
    {
        cerr << "Usage: <file_type:text|binary> <mode:random|sparse|eye|01> <size> <length>" << endl;
        exit(1);
    }
    try
    {
        file_type = argv[1];
        if (file_type != "binary" && file_type != "text")
            throw argv[1];
        mode = argv[2];
        if (mode != "random" && mode != "sparse" && mode != "eye" && mode != "01")
            throw argv[2];
        size = atoi(argv[3]);
        length = atoi(argv[4]);
    }
    catch (char *argument)
    {
        cerr << "Argument \'" << argument << "\' invalid" << endl;
        exit(1);
    }
}

void save_text(int32_t **result)
{
    ostr << ".txt";
    ofstream fout;
    fout.open(ostr.str());
    fout << size << endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            fout << result[i][j] << " ";
        }
        fout << endl;
    }
    fout.close();
}

void save_binary(int32_t **result)
{
    ostr << ".bin";
    ofstream fout;
    fout.open(ostr.str(), ios::binary);
    fout.write((char *)&size, sizeof(uint32_t));
    for (int i = 0; i < size; i++)
        fout.write((char *)result[i], size * sizeof(int32_t));
    fout.close();
}

void generate_random(int32_t **result)
{
    parallel_for(int i = 0; i < size; i++)
        parallel_for(int j = 0; j < size; j++)
            result[i][j] = rand() % mod;
}

void generate_sparse(int32_t **result)
{
    parallel_for(int i = 0; i < size; i++)
        parallel_for(int j = 0; j < size; j++)
            result[i][j] = (rand() % 100 < 10) ? (rand() % mod) : 0;
}

void generate_eye(int32_t **result)
{
    parallel_for(int i = 0; i < size; i++)
        parallel_for(int j = 0; j < size; j++)
            result[i][j] = (i == j) ? 1 : 0;
}

void generate_01(int32_t **result)
{
    parallel_for(int i = 0; i < size; i++)
        parallel_for(int j = 0; j < size; j++)
            result[i][j] = (i * size + j) % 2;
}
