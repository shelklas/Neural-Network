#include <iostream>
#include <functional>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

const double EulerConstant = std::exp(1.0);

class Neural_Network
{
public:
	int m_layerSize;
	vector<vector<float>> m_weights;
	int m_flag; // Sets the type of layer (0 is input, 1 is hidden,2 is output)
	vector<float> m_input;
	Neural_Network(int layerSize, vector<vector<float>> weights, int flag) : m_layerSize(layerSize), m_weights(weights), m_flag(flag) {
		for (auto a = 0; a < m_layerSize; ++a)
			m_input.push_back(0);
	}
};

void readfromFile(int& networkSize, vector<int>& networkLayer, vector<string>& hiddenLayerNames, vector<vector<vector<float>>>& weights)
{
	int tempInt;
	vector<float> weightHold;
	vector < vector<float>> layerWeightHold;
	string nameHold;
	float weightTemp;

	ifstream ifs;
	try
	{
		ifs.open("NN.cia");
	}
	catch (...)
	{
		cout << "Error: Failed Reading file!" << endl;
		//return EXIT_FAILURE;
	}

	ifs >> networkSize;
	if (networkSize < 3)
	{
		cout << "ERROR: NETWORK SIZE MUST BE LARGER THAN 3..." << endl;
		//return EXIT_FAILURE;
	}
	// Read network layers
	for (auto i = 0; i < networkSize; ++i)
	{
		ifs >> tempInt;
		networkLayer.push_back(tempInt);
	}

	// Read names for the Output Layer
	for (int i = 0; i < networkLayer.back(); ++i)
	{
		ifs >> nameHold;
		hiddenLayerNames.push_back(nameHold);
	}

	// Read in all weights. Weights are also put into each containing weight layer
	for (auto i = 0; i < networkLayer.size() - 1; ++i)
	{
		layerWeightHold.clear();
		for (auto j = 0; j < networkLayer[i]; ++j)
		{
			weightHold.clear();
			for (auto k = 0; k < networkLayer[i + 1]; ++k)
			{
				ifs >> weightTemp;
				weightHold.push_back(weightTemp);
			}
			layerWeightHold.push_back(weightHold);

		}
		weights.push_back(layerWeightHold);
	}
}

void createNeuralNetwork(vector<float>& initial, int & networkSize, vector<int>& networkLayer, vector<vector<vector<float>>>& weights, vector<Neural_Network>& network)
{
	vector<vector<float>> layerWeight;

	// Create input layer in NN
	network.push_back(Neural_Network(networkLayer.front(), weights.front(), 0));
	network[0].m_input = initial;

	// Creating all hidden layers in NN
	for (auto i = 1; i < networkSize - 1; ++i)
		network.push_back(Neural_Network(networkLayer[i], weights[i], 1));

	// Creating output layer
	network.push_back(Neural_Network(networkLayer.back(), weights.back(), 2));
}
void forwardPropogate(vector<Neural_Network>& network)
{
	for (auto i = 1; i < network.size() - 1; ++i)
	{
		for (auto a = 0; a < network[i - 1].m_layerSize; ++a)
			for (auto j = 0; j < network[i].m_layerSize; ++j)
				network[i].m_input[j] += (network[i - 1].m_input[a] * network[i].m_weights[a][j]);
		//Do sigmoid transform
		if (network[i].m_flag == 1) // WE DOING MATH UP IN HERE
			for (auto t = 0; t < network[i].m_input.size(); ++t)
				network[i].m_input[t] = float(1 / (1 + pow(EulerConstant, -(network[i].m_input[t]))));
	}

	// DEBUG TOOLS! (PRINTS FINAL LAYER OUTPUT)
	//cout << "i = " << i << ": j = " << j << ": k = " << k << endl;
	//cout << "LAYER: " << i << " NEURON: " << j  << ": " << network[i + 1].m_input[j] << endl;
	//cout << "i = " << i << ": j = " << j << ": k = " << k << ": a = " << a << endl;
	for (vector<float>::iterator it = network.back().m_input.begin(); it != network.back().m_input.end(); ++it)
		cout << "Neuron " << distance(network.back().m_input.begin(), it) << " : " << *it << endl;
}
int main()
{
	int networkSize;
	vector<int> networkLayer;
	vector<string> hiddenLayerNames;
	vector<vector<vector<float>>> weights;
	vector<Neural_Network> network;

	vector<float> inititalValue = {0,0,0,0,0 };
	cout << "Welcome to Sheldon's Neural Network!" << endl;
	cout << "Reading File..." << endl;
	readfromFile(networkSize, networkLayer, hiddenLayerNames, weights);
	cout << "Reading Complete..." << endl;

	cout << "Setting Up Network..." << endl;
	createNeuralNetwork(inititalValue, networkSize, networkLayer, weights, network);
	cout << "Network Created..." << endl;

	cout << "Propogate Forward..." << endl;
	forwardPropogate(network);

	cout << "Propogation Complete..." << endl;
	cout << "Calculating Error..." << endl;

}