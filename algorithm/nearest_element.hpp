/* @file nearest_element.hpp
 * @brief 自分の要素より大きい/小さい最も近い要素を探す
 * @author m1ffyz
 * @date 2026-02-03
 * @see https://github.com/m1ffyz/library/blob/main/algorithm/nearest_element.hpp
 */


// T : データの型
// rev : true なら右側の要素を探索 (default : 左側)
// pred : bool pred(stack_val, current_val) が true ならその stack の要素は候補として残る
template <typename T, typename Pred>
vector<int> nearest_element(const vector<T> &data, Pred pred, bool rev = false, int not_found = -1) {
    int n = data.size();
    vector<int> result(n);
    stack<int> st;

    int start = n - 1;
    int end = -1;
    int step = -1;
    if (!rev) {
        start = 0;
        end = n;
        step = 1;
    }

    for (int i = start; i != end; i += step) {
        while (!st.empty() and !pred(data[st.top()], data[i])) {
            st.pop();
        }
        if (!st.empty()) {
            result[i] = st.top();
        } else {
            result[i] = not_found;
        }
        st.push(i);
    }

    return result;
} 

// 例 : 自分以下の値を探す場合 -> return stack_val <= current_val;
bool pred(long long stack_val, long long current_val) {
    return stack_val <= current_val;
}

