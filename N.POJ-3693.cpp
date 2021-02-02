#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
#define F(x) ((x) / 3 + ((x) % 3 == 1 ? 0 : tb))
#define G(x) ((x) < tb ? (x) * 3 + 1 : ((x) - tb) * 3 + 2)
using namespace std;
const int maxn = 100005;
int wa[maxn], wb[maxn], wc[maxn], wv[maxn], sa[3 * maxn];
int r[maxn], h[maxn], s[3 * maxn];
int n;
char ch[maxn];
int d[maxn][30];
vector<int> res;

int c0(int *s, int a, int b) {
    return s[a] == s[b] && s[a + 1] == s[b + 1] && s[a + 2] == s[b + 2];
}

int c12(int k, int *s, int a, int b) {
    if (k == 2)
        return s[a] < s[b] || s[a] == s[b] && c12(1, s, a + 1, b + 1);
    return s[a] < s[b] || s[a] == s[b] && wv[a + 1] < wv[b + 1];
}

void sort(int *s, int *a, int *b, int n, int m) {
    for (int i = 0; i < n; i++) wv[i] = s[a[i]];
    for (int i = 0; i < m; i++) wc[i] = 0;
    for (int i = 0; i < n; i++) wc[wv[i]]++;
    for (int i = 1; i < m; i++) wc[i] += wc[i - 1];
    for (int i = n - 1; i >= 0; i--) b[--wc[wv[i]]] = a[i];
}

void dc3(int *s, int *sa, int n, int m) {
    int i, j, *sn = s + n, *san = sa + n, ta = 0, tb = (n + 1) / 3, tbc = 0, p;
    s[n] = s[n + 1] = 0;
    for (i = 0; i < n; i++) if (i % 3 != 0) wa[tbc++] = i;
    sort(s + 2, wa, wb, tbc, m);
    sort(s + 1, wb, wa, tbc, m);
    sort(s, wa, wb, tbc, m);
    for (p = 1, sn[F(wb[0])] = 0, i = 1; i < tbc; i++)
        sn[F(wb[i])] = c0(s, wb[i - 1], wb[i]) ? p - 1 : p++;
    if (p < tbc) dc3(sn, san, tbc, p);
    else for (i = 0; i < tbc; i++) san[sn[i]] = i;
    for (i = 0; i < tbc; i++) if (san[i] < tb) wb[ta++] = san[i] * 3;
    if (n % 3 == 1) wb[ta++] = n - 1;
    sort(s, wb, wa, ta, m);
    for (i = 0; i < tbc; i++) wv[wb[i] = G(san[i])] = i;
    for (i = 0, j = 0, p = 0; i < ta && j < tbc; p++)
        sa[p] = c12(wb[j] % 3, s, wa[i], wb[j]) ? wa[i++] : wb[j++];
    for (; i < ta; p++) sa[p] = wa[i++];
    for (; j < tbc; p++) sa[p] = wb[j++];
}

void getheight() {
    int i, j, k = 0;
    for (i = 1; i <= n; i++) r[sa[i]] = i;
    for (i = 0; i < n; h[r[i++]] = k)
        for (k ? k-- : 0, j = sa[r[i] - 1]; s[i + k] == s[j + k]; k++);
}

void RMQ_init(){
    for(int i = 1; i <= n; i++)d[i][0] = h[i];
    for(int j = 1; (1 << j) <= n; j++){
        for(int i = 1; i + (1 << j) - 1 <= n; i++){
            d[i][j] = min(d[i][j - 1], d[i + (1 << (j - 1))][j - 1]);
        }
    }
}

int RMQ(int l, int r){
    if(l > r) swap(l, r);
    int k = 0;
    while((1 << (k + 1)) <= r - l + 1)k++;
    return min(d[l][k], d[r - (1 << k) + 1][k]);
}

int query(int L, int R){
    int x = r[L], y = r[R];
    if(x > y) swap(x, y);
    x++;
    return RMQ(x, y);
}

int main(){
    int kase = 0;
    while(~scanf("%s", ch) && ch[0] != '#'){
        n = strlen(ch);
        for(int i = 0; i < n; i++)s[i] = ch[i] - 'a' + 1;
        s[n] = 0;
        dc3(s, sa, n + 1, 40);
        getheight();
        RMQ_init();
        int ans = 1;
        for(int len = 1; len <= n; len++){
            for(int j = 0; j + len < n; j += len){
                int lcp = query(j, j + len);
                int t = lcp / len + 1;
                int k = j - (len - lcp % len);
                if(k >= 0 && query(k, k + len) >= len)t++;
                if(ans < t){
                    ans = t;
                    res.clear();
                    res.push_back(len);
                }
                else if(ans == t){
                    res.push_back(len);
                }
            }
        }
        printf("Case %d: ", ++kase);
        for(int i = 1; i <= n; i++){
            bool flag = false;
            for(int j = 0; j < res.size(); j++){
                //printf("%d\n", res[j]);
                if(query(sa[i], sa[i] + res[j]) >= (ans - 1) * res[j]){
                    flag = true;
                    for(int k = sa[i]; k < sa[i] + res[j] * ans; k++){
                        putchar(ch[k]);
                    }
                    puts("");
                    break;
                }
            }
            if(flag)break;
        }
    }
    return 0;
}