#include "Calculator.cpp"
#include "Tokenizer.cpp"
#include "AST.cpp"

#include <string>

int main()
{
    Calculator c;
    vector<string> input;
    vector<string> output;
    while(c.flag != true){
        cin >> c;
        input.push_back(c.input_);
        output.push_back(c.output_);
    }

    for(int i = 0; i < output.size() - 1; i++){
        cout << "in [" << i <<"]: " << input[i] << endl;
        cout << "out [" << i <<"]: " << output[i] << endl;
    }   
    return 0;
}