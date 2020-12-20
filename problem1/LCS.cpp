#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <unordered_map>

using namespace std;

short s[101][101];
unordered_map<short,vector<pair<short,short>>> p;
string x,y;
vector<string> output;
short m,n; 

void backtrace(string lcs, short r, short c, short idx){
	if(idx == 0){
		output.push_back(lcs);
	}else{
		for(vector<pair<short,short>>::iterator it=p[s[r][c]].begin(); it!=p[s[r][c]].end(); it++){
			if(it->first <= r && it->second <= c){
				lcs[idx-1] = x[it->first-1];
				backtrace(lcs, it->first-1, it->second-1, idx-1);
			}
		}
	}
}

int main(){
	ios_base::sync_with_stdio(false); 
    cin.tie(NULL);
	memset(s,0,sizeof(s));
    cin >> x >> y;
    m = x.size();
    n = y.size();
    for(short i=1; i<=m; i++){
        for(short j=1; j<=n; j++){
			if(x[i-1]==y[j-1]){
				s[i][j] = s[i-1][j-1]+1;
				p[s[i][j]].push_back(pair<short,short>(i,j));
			}else{
				s[i][j] = max(s[i-1][j],s[i][j-1]);
			}
		}
	}

	backtrace(string(s[m][n],32), m, n, s[m][n]);
	sort(output.begin(), output.end());
	
    cout << s[m][n] << " " << output.size() << "\n";
    for(vector<string>::iterator it = output.begin(); it<output.end(); it++)
        cout << *it << "\n";
	
    return 0;
}