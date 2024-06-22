bool ans_check(long long x){

}

long long ans_binary_search(){
    long long left = -1, right = 1e18;
    if(ans_check(right)){
        while(right - left > 1){
            long long mid = (left + right) / 2;
            // cout << "debug: left = " << left << ", mid = " << mid << ", right = " << right << " ans(" << mid << ") = " << ans_check(mid) << endl;
            if(ans_check(mid)) right = mid;
            else left = mid;
        }
        return right;
    }else{
        while(right - left > 1){
            long long mid = (left + right) / 2;
            // cout << "debug: left = " << left << ", mid = " << mid << ", right = " << right << " ans(" << mid << ") = " << ans_check(mid) << endl;
            if(ans_check(mid)) left = mid;
            else right = mid;
        }
        return left;
    }
}
