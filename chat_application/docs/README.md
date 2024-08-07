
Cấu trúc chương trình

main thread có listenfd để accept -> mỗi lần accept thì sẽ create ra 1 thread handle sockfd đấy 
có 1 thread để handle người dùng nhập từ bàn phím

có biến global để giám sát số lượng peers kết nối tới -> cần đồng bộ bàng mutex 