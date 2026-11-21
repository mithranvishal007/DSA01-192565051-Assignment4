#include <iostream>
#include <iomanip>
#include <array>
#include <string>
#include <limits>
#include <cstdint>
#include <algorithm>

enum class Zone { Inner=0, Middle=1, Outer=2, Airport=3 };
constexpr std::array<long long,4> BASE_PAISE{1000,2000,3000,6000};
constexpr int PEAK_PERCENT=20;
constexpr long long MIN_REMAINING=5000;
constexpr int CONCESSION_PERCENT=10;

bool isPeak(int hour){ return (hour>=8 && hour<10)||(hour>=17 && hour<20); }
long long fare(Zone z,int hour,bool concession=false,bool weekend=false){
    auto idx=static_cast<std::size_t>(z);
    if(idx>=BASE_PAISE.size()) return -1;
    long long amount=BASE_PAISE[idx];
    if(isPeak(hour)) amount += amount*PEAK_PERCENT/100;
    if(concession) amount -= amount*CONCESSION_PERCENT/100;
    if(weekend) amount -= amount*10/100; // design challenge: weekend discount replaces peak surcharge below in revised model
    return amount;
}
bool canTravel(long long balance,long long f){return f>=0 && balance-f>=MIN_REMAINING;}
class SmartCard{
    long long balance_;
public:
    explicit SmartCard(long long initial):balance_(std::max(0LL,initial)){}
    long long balance() const{return balance_;}
    bool deduct(long long f){if(!canTravel(balance_,f))return false; balance_-=f;return true;}
};
std::string zoneName(Zone z){switch(z){case Zone::Inner:return "Inner";case Zone::Middle:return "Middle";case Zone::Outer:return "Outer";case Zone::Airport:return "Airport";}return "Unknown";}
void printMoney(long long paise){std::cout<<"Rs "<<paise/100<<'.'<<std::setw(2)<<std::setfill('0')<<paise%100<<std::setfill(' ');}
int main(){
    std::cout<<"Metro Smart-Card Fare Demonstration\\n";
    SmartCard card(10000);
    struct Trip{Zone z;int hour;bool concession;};
    Trip trips[]={{Zone::Inner,11,false},{Zone::Outer,18,false},{Zone::Middle,9,true}};
    for(const auto&t:trips){
        long long f=fare(t.z,t.hour,t.concession);
        std::cout<<zoneName(t.z)<<" at "<<t.hour<<":00: ";
        if(card.deduct(f)){std::cout<<"fare ";printMoney(f);std::cout<<", balance ";printMoney(card.balance());}
        else std::cout<<"denied; balance ";printMoney(card.balance());
        std::cout<<"\\n";
    }
    std::cout<<"Peak boundary checks (8,10,17,20): ";
    for(int h:{8,10,17,20}) std::cout<<isPeak(h)<<" ";
    std::cout<<"\\n";
}
