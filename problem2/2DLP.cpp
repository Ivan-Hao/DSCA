#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <set>
#include <tuple>
#include <list>

#define EPSILON numeric_limits<double>::epsilon()
#define NOFEASIBLEREGION numeric_limits<int>::max()-20
#define NEGATIVEINFINITY numeric_limits<int>::lowest()+20
#define DBL_MAX numeric_limits<double>::max()
#define DBL_MIN numeric_limits<double>::lowest()
#define INT_MAX numeric_limits<int>::max()
#define INT_MIN numeric_limits<int>::lowest()
#define RIGHT numeric_limits<int>::max()-10
#define LEFT numeric_limits<int>::lowest()+10

using namespace std;

struct constraint{
    double a;
    double b;
    double c;
};
unordered_map<int,list<constraint>> subject;

inline double intersection_x( const constraint &l1, const constraint &l2 ){ return ( l2.b * l1.c - l1.b * l2.c ) / ( l1.a * l2.b - l2.a * l1.b );}
inline double intersection_y( const constraint &l1, const constraint &l2 ){ return ( l1.a * l2.c - l2.a * l1.c ) / ( l1.a * l2.b - l2.a * l1.b );}
inline bool is_parallel( const constraint &l1, const constraint &l2 ){ return fabs(l1.a/l1.b - l2.a/l2.b) < EPSILON ;}
inline double slope(const constraint &l1){ return -l1.a / l1.b; }
inline double obY( const constraint &l1, double x ){ return ( l1.c - l1.a * x ) / l1.b;}
inline double obX( const constraint &l1, double y ){ return ( l1.c - l1.b * y ) / l1.a;}
inline bool belong(const constraint &l1, double x, double y){ return fabs(l1.a * x + l1.b * y - l1.c) < 1;}
inline int classfier( const constraint &l1 ){
    if( l1.b < 0 ) return -1;
    if( l1.b > 0) return 1;
    return 0;
}

inline void interval( const list<constraint> &I0, double &xl, double &xr ){
    for(list<constraint>::const_iterator it = I0.begin(); it != I0.end(); it++){
        if( it->a > 0 ) xr = min( it->c/it->a, xr);
        else xl = max( it->c/it->a, xl);
    }
}

inline int observation( double x ){
    double alpha_y = DBL_MIN;
    double beta_y = DBL_MAX;
    double t_min_slope = DBL_MAX;
    double t_max_slope = DBL_MIN;
    double s_min_slope = DBL_MAX;
    double s_max_slope = DBL_MIN;
    for(auto it=subject[1].begin(); it!=subject[1].end(); it++) beta_y = min(beta_y ,obY(*it, x));
    for(auto it=subject[-1].begin(); it!=subject[-1].end(); it++) alpha_y = max(alpha_y ,obY(*it, x));
    for(auto it=subject[1].begin(); it!=subject[1].end(); it++){
        if( !belong(*it, x, beta_y) ) continue;
        double rate = slope(*it);
        t_max_slope = max(t_max_slope,rate);
        t_min_slope = min(t_min_slope,rate);
    }
    for(auto it=subject[-1].begin(); it!=subject[-1].end(); it++){
        if( !belong(*it, x, alpha_y) ) continue;
        double rate = slope(*it);        
        s_min_slope = min(s_min_slope,rate);
        s_max_slope = max(s_max_slope,rate);
    }
    if( alpha_y <= beta_y && s_max_slope < 0 ) return RIGHT;
    if( alpha_y <= beta_y && s_min_slope > 0 ) return LEFT;
    if( alpha_y <= beta_y ) return int(alpha_y);
    if( s_max_slope < t_min_slope ) return RIGHT;
    if( s_min_slope > t_max_slope) return LEFT;
    return NOFEASIBLEREGION;
}

inline double solve(list<constraint> &con, double xl, double xr){
    list<constraint>::iterator it1 = con.begin();
    list<constraint>::iterator it2 = ++con.begin();
    if( slope(*it1) < slope(*it2) ) swap( it1, it2 );
    double s_1 = slope(*it1);
    double s_2 = slope(*it2);
    if( s_1 == s_2 ){
        double y1 = obY( *it1, 0);
        double y2 = obY( *it2, 0);
        if ( it1->b < 0 && it2->b > 0 ){
            if( y1 > y2 ) return DBL_MAX;
            return ( s_1 > 0 ) ? obY(*it1, xl): obY(*it1, xr);
        }else if( it1->b > 0 && it2->b < 0 ){
            if( y2 > y1 ) return DBL_MAX;
            return ( s_2 > 0 ) ? obY(*it2, xl) : obY(*it2, xr);
        }else{ 
            if( y1 > y2 ) return ( s_1 > 0 ) ? obY(*it1, xl): obY(*it1, xr);
            else return ( s_2 > 0 ) ? obY(*it2, xl): obY(*it2, xr);
        }
    }else if( s_1 < 0 ){ 
        if ( it1->b > 0 && it2->b < 0 ){ 
            if( xr != DBL_MAX ) return obY(*it2, xr);
            return DBL_MIN;
        }else if ( it1->b < 0 && it2->b > 0 ){ 
            double x = intersection_x( *it1, *it2 );
            if( x < xl ) return DBL_MAX;
            if( x > xr ) return obY(*it1, xr);
            return intersection_y( *it1, *it2 );
        }else{ 
            if( xr != DBL_MAX ) return obY(*it1, xr);
            return DBL_MIN;
        }
    }else if( s_2 > 0 ){
        if( it1->b < 0 && it2->b > 0 ){ 
            if( xl != DBL_MIN ) return obY(*it1, xl);
            return DBL_MIN;
        }else if( it1->b > 0 && it2->b < 0 ){ 
            double x = intersection_x( *it1, *it2 );
            if( x > xr ) return DBL_MAX;
            if( x < xl ) return obY(*it2, xl);
            return intersection_y( *it1, *it2 );
        }else{ 
            if( xl != DBL_MIN ) return obY(*it2, xl);
            return DBL_MIN;
        }
    }else{
        if( it1->b > 0 && it2->b < 0 ){ 
            if( xr != DBL_MAX ) return obY(*it2, xr);
            return DBL_MIN;
        }else if( it1->b < 0 && it2->b > 0 ){ 
            if( xl != DBL_MIN ) return obY(*it1, xl);
            return DBL_MIN;
        }else{ 
            double x = intersection_x( *it1, *it2 );
            if( x < xl ) return obY(*it1, xl);
            if( x > xr ) return obY(*it2, xr);
            return intersection_y( *it1, *it2 );
        }
    }
}

inline void pruneConstraint(const list< tuple<int,list<constraint>::iterator,double,list<constraint>::iterator,double,double>> &rx, double median, bool r_l){
    if(r_l){ // prune left
        for(auto it=rx.begin(); it!=rx.end(); it++){
            if(get<5>(*it) > median) continue;
            if(get<0>(*it) == 1){ 
                if(get<2>(*it) > get<4>(*it)) subject[1].erase( get<1>(*it) );
                else subject[1].erase( get<3>(*it) );
            }else{ 
                if(get<2>(*it) < get<4>(*it)) subject[-1].erase( get<1>(*it) );
                else subject[-1].erase( get<3>(*it) );
            }
        }
    }else{ // prune right
        for(auto it=rx.begin(); it!=rx.end(); it++){
            if(get<5>(*it) < median) continue;
            if(get<0>(*it) == 1){
                if(get<2>(*it) < get<4>(*it)) subject[1].erase( get<1>(*it) );
                else subject[1].erase( get<3>(*it));
            }else{
                if(get<2>(*it) > get<4>(*it)) subject[-1].erase( get<1>(*it) );
                else subject[-1].erase( get<3>(*it));
            }
        }
    }
}

int main(){
    int num;
    cin >> num;
    for(int i=0; i<num; i++){
        constraint equation;
        cin >> equation.a >> equation.b >> equation.c;
        subject[classfier(equation)].push_back(equation);
    }
    double xl = DBL_MIN;
    double xr = DBL_MAX;
    interval(subject[0], xl, xr);
    
    while( !subject[-1].empty() && (subject[1].size() + subject[-1].size()) > 2 ){
        set<double> median;
        list< tuple<int, list<constraint>::iterator, double, list<constraint>::iterator ,double, double> > rx;
        list<constraint>::iterator it1, it2;
        for( it1=subject[1].begin(),it2=++subject[1].begin(); it1!=subject[1].end() && it2!=subject[1].end(); ){
            if( is_parallel( *it1,*it2 ) ){
                if( obY(*it1,0) > obY(*it2,0) ){
                    it1 = subject[1].erase(it1);
                    it2++;
                }else{
                    it2 = subject[1].erase(it2);  
                }
                continue;
            }
            double x_coordinate = intersection_x( *it1,*it2 );
            double s_it1 = slope(*it1);
            double s_it2 = slope(*it2);
            if( x_coordinate > xr ){
                if(  s_it1 > s_it2  ){
                    it2 = subject[1].erase(it2);
                }else{
                    it1 = subject[1].erase(it1);
                    it2++;
                }
                continue;
            }else if( x_coordinate < xl ){
                if( s_it1 > s_it2 ){
                    it1 = subject[1].erase(it1);
                    it2++;                    
                }else{
                    it2 = subject[1].erase(it2);
                }
                continue;
            }
            rx.push_back( make_tuple(1,it1,s_it1,it2,s_it2,x_coordinate) );
            median.insert(x_coordinate);
            ++(++it1);++(++it2);
        }
        for( it1=subject[-1].begin(),it2=++subject[-1].begin(); it1!=subject[-1].end() && it2!=subject[-1].end(); ){
            if( is_parallel( *it1,*it2 ) ){
                if( obY(*it1,0) < obY(*it2,0) ){
                    it1 = subject[-1].erase(it1);
                    it2++;
                }else{
                    it2 = subject[-1].erase(it2);  
                }
                continue;
            }
            double x_coordinate = intersection_x( *it1,*it2 );
            double s_it1 = slope(*it1);
            double s_it2 = slope(*it2);
            if( x_coordinate > xr ){
                if( s_it1 > s_it2 ){
                    it1 = subject[-1].erase(it1);
                    it2++;
                }else{
                    it2 = subject[-1].erase(it2);
                }
                continue;
            }else if( x_coordinate < xl ){
                if( s_it1 > s_it2 ){
                    it2 = subject[-1].erase(it2);
                }else{
                    it1 = subject[-1].erase(it1);
                    it2++; 
                }
                continue;
            }
            rx.push_back( make_tuple(-1,it1,s_it1,it2,s_it2,x_coordinate) );
            median.insert(x_coordinate);
            ++(++it1);++(++it2);
        }
        if(median.empty()) continue;
        set<double>::iterator it_median = median.begin();
        std::advance(it_median, median.size()/2);
        int result = observation(*it_median);
        switch(result){
            case RIGHT:
                xl = *it_median;
                pruneConstraint( rx, *it_median, true);
                break;
            case LEFT:
                xr = *it_median;
                pruneConstraint( rx, *it_median, false);
                break;
            case NOFEASIBLEREGION:
                cout << "NA" << "\n";
                return 0;
            case NEGATIVEINFINITY:
                cout << "-INF" << "\n";
                return 0;
            default:
                cout << result <<"\n";
                return 0;
        }
    }
    
    if( xr < xl ){
        cout << "NA\n"; 
        return 0;
    }
    if(subject[-1].empty()){
        cout << "-INF\n"; 
        return 0;
    }

    list<constraint> con;
    con.insert( con.end(), subject[1].begin(), subject[1].end());
    con.insert( con.end(), subject[-1].begin(), subject[-1].end());

    int result_y = round(solve(con, xl, xr));    
    if( result_y == INT_MAX ) cout << "NA\n";
    else if( result_y == INT_MIN ) cout << "-INF\n";
    else cout << result_y << "\n";

    return 0;
}