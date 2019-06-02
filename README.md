# FlowPractice
## maxflow
Độ khó: 5.9

Đây là bài dùng để kiểm tra độ đúng đắn của cách cài đặt thuật tìm luồng cực đại trên mạng của bạn. Các bạn chỉ cần xây dựng luồng theo như đề bài và in ra các cạnh có luồng đi qua lớn hơn 0.

Thuật luồng mình sử dụng để AC bài này là thuật đẩy tiền luồng với mẹo cài đặt đẩy nhãn theo khe được trình bày trong sách Tài liệu giáo khoa chuyên Tin quyển 2. 
## mazemovement
Độ khó: 3.0

Bạn cứ xây dựng luồng theo hướng dẫn của đề bài. 

Một số lưu ý:
* Đỉnh nguồn của mạng là đỉnh có giá trị nhỏ nhất (không phải đỉnh đầu tiên), và đỉnh thu là đỉnh có giá trị lớn nhất (không phải đỉnh cuối cùng
* Chỉ thêm cạnh vào mạng nếu khả năng thông qua lớn hơn 1.
* Cạnh trong mạng là cạnh hai chiều, không phải cạnh một chiều như thông thường.

Thuật luồng mình sử dụng trong bài này: Đẩy tiền luồng + đẩy nhãn theo khe
## minimumcut
Độ khó: 4.2

Đây là bài dùng để kiểm tra độ đúng đắn của cách cài đặt thuật tìm lát cắt cực tiểu.

Để tìm được tập U trong đề bài, ta làm như sau:
* Xây dựng mạng như đề bài rồi tìm luồng cực đại trên mạng.
* Từ đỉnh nguồn, ta đi đến các đỉnh khác qua các cạnh mà luồng đi qua cạnh nhỏ hơn khả năng thông qua của cạnh đó.
* Các đỉnh được thăm sẽ thuộc tập U.

Một số lưu ý trong bài này:
* Cạnh được cho trong đề là from u to v, nghĩa là cạnh một chiều.
* Ta cần in ra số phần tử của tập U, không phải tổng trọng số các cạnh trong lát cắt cực tiểu.

Mở rộng: 
* Để tìm lát cắt cực tiểu, ta tìm các cạnh nối giữa một đỉnh thuộc tập U và một đỉnh không thuộc tập U
* Tổng trọng số các cạnh trong lát cắt cực tiểu bằng giá trị luồng cực đại trên mạng.
