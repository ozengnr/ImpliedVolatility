/* IMPLIED VOLATILITY */

#include <iostream>
#include <cmath>
using namespace std;

//function prototypes

double EuroCall (double S, double K, double r, double q, double T, double sig);
double EuroPut (double S, double K, double r, double q, double T, double sig);
double NP (double x);
double N (double x);

int main() 
{
    double S = 69.77, K = 69.77, r = 0.01024, q = 0.01014, T = 1.0; //no sigma because we're calculating implied volatility

    double sig, optPrice, testPrice, stepSize, stepSign;
    char optType; //'c' for call and 'p' for put
    int i, iterFlag;

    optPrice = 5.29; //implied volatility should give this option price
    optType = 'c'; //implied volatility for a call option 

    sig = 0.;
    stepSize = 0.1; //sensitivity
    iterFlag = 1; //when to stop iteration
    stepSign = 1.; //control direction and size of steps

    for (i = 1; i < 30 && iterFlag < 4; i++)
    {
        sig += stepSize;

        switch (optType)
        {
        case 'c':
            testPrice = EuroCall(S,K,r,q,T,sig);
            break;
        
        case 'p':
            testPrice = EuroPut(S,K,r,q,T,sig);
            break;
        }

        cout << i << " " << sig << " " << testPrice << " ";
        cout << stepSize << endl;

        if ((testPrice * stepSign) > (optPrice * stepSign))
        {
            cout << endl;
            stepSign = -stepSign;
            stepSize = (stepSize * 0.1);
            iterFlag++; //increasing function of a volatility
        }
    } 

    cout << "\nImplied volatility: " << sig << endl;
    cout << "Specified option price: " << optPrice << endl;
    cout << "Option price at implied volatility: " << testPrice << "\n";
    
    cout << endl << endl;
    return 0;
}

/* funtion definitions */
double NP (double x)
{
    return (1.0/sqrt(2.0*3.1415)*exp(-x*x*0.5));
}

double N (double x)
{
    double a1 = 0.319381530;
    double a2 = -0.356563782;
    double a3 = 1.781477937;
    double a4 = -1.821255978;
    double a5 = 1.330274429;
    double k;

    k = 1/(1+0.2316419*x);

    if (x >= 0.0)
    {
        return (1-NP(x)*((a1*k)+(a2*k*k)+(a3*k*k*k)+(a4*k*k*k*k)+(a5*k*k*k*k*k)));
    }
    else
    {
        return (1-N(-x));
    }
    

}

double EuroCall(
    double S,
    double K,
    double r,
    double q,
    double T,
    double sig
)
{
    double d1, d2;
    d1 = (log(S/K) + (r-q+(sig*sig)*0.5)*T) / (sig*sqrt(T));
    d2 = d1 - sig*sqrt(T);
    return S*exp(-q*T) * N(d1) - K*exp(-r*T) * N(d2);
}

double EuroPut(
    double S,
    double K,
    double r,
    double q,
    double T,
    double sig
)
{
    double d1, d2;
    d1 = (log(S/K) + (r-q+(sig*sig)*0.5)*T) / (sig*sqrt(T));
    d2 = d1 - sig*sqrt(T);
    return K*exp(-r*T) * N(-d2) - S*exp(-q*T) * N(-d1);
}
