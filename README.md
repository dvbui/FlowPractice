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

Thuật luồng mình sử dụng trong bài này: Đẩy tiền luồng + đẩy nhãn theo khe
## dutyscheduler
Độ khó: 3.1

Đầu tiên, ta thấy ta cần phải tối thiểu số ngày tối đa mà một RA phải phục vụ cho trường. Sau một hồi suy nghĩ, ta thấy ngay được ta có thể sử dụng thuật toán tìm kiếm nhị phân để cố định số ngày tối đa mà một RA phải phục vụ. (Các bài luồng rất hay có kiểu chặt nhị phân như thế này, các bạn nên lưu ý).

Vấn đề là ta kiểm tra số ngày tối đa mà một RA phải phục vụ có hợp lệ hay không như thế nào?

Ta có thể quy vấn đề trên về dạng bài ghép cặp, trong đó các phần tử được chia thành hai tập A và tập B, mỗi phần tử của tập A có thể ghép với n phần tử của tập B, mỗi phần tử của tập B có thể được ghép với m phần tử của tập A. Ngoài ra, một cặp phần tử một trong tập A và một trong tập B chỉ được ghép với nhau một lần. Đây là một dạng bài có thể được giải quyết bằng luồng cực đại trên mạng.

Để giải bài toán tổng quát, ta xây dựng mạng như hình dưới đây:

(insert hình ảnh mạng mình sẽ vẽ sau)

rồi chạy thuật toán tìm luồng cực đại. Để kiểm tra xem một cặp phần tử một trong tập A và một trong tập B có được nối với nhau hay không, ta kiểm tra xem có luồng đi qua cạnh tương ứng với phần tử đó hay không.

Quay trở lại bài toán chính, ta có thể coi mỗi RA là một phần tử trong tập A, mỗi ngày là một phần tử trong tập B. Ta đặt n là số ngày tối đa mà một RA có thể phục vụ và m=2 là số RA mà mỗi ngày cần. Để kiểm tra n có hợp lệ hay không, ta chạy thuật tìm luồng cực đại trên mạng rồi kiểm tra xem cạnh nối từ các đỉnh trong tập B đến đỉnh thu có bão hòa (luồng đi qua bằng khả năng thông qua) hay không.

(Mỗi lần kiểm tra bạn có thể xây dựng lại mạng, hoặc khởi tạo lại các giá trị quan trọng trong mạng đang có)

Sau khi tìm được số n thích hợp bằng thuật toán tìm kiếm nhị phân, ta xây dựng lại mạng và chạy thuật luồng một lần cuối cùng. Sau đó, bạn chỉ việc kiểm tra xem một ngày được nối với những RA nào và in ra kết quả.

## jupiter
Độ khó: 5.1

Bài này hơi khó đọc nên mình tóm tắt ngắn gọn lại đề bài cho các bạn:

Cho Q hàng đợi, hàng đợi thứ i có dung lượng c(i) thông tin, và N cơ hội để truyền thông tin về Trái Đất, trước mỗi cơ hội mỗi hàng đợi sẽ nhận được một lượng thông tin nhất định (trong lúc nhận thông tin các hàng đợi không được phép truyền về), và ở mỗi cơ hội, các hàng đợi sẽ chỉ có thể truyền về tổng cộng d(i) thông tin.

Ta tưởng tượng luồng trên mạng là các dòng thông tin và nghĩ ra mạng dưới đây (thực ra mình nghĩ được cũng là do nó thuộc một dạng mà mình đã làm rồi, nếu bạn nào chưa học luồng mà chưa tưởng tượng hay nghĩ ra được thì cũng không sao cả :3)

(insert ảnh từ solution thật)

Ảnh được lấy từ bản solution tiếng Anh (bài J trong này: http://2016.nwerc.eu/nwerc2016slides.pdf)

## piano
Độ khó: 4.3

Lưu ý: Các thợ sửa piano làm việc theo cặp, nên ta có thể ghép hai thợ sửa piano vào làm một để điều kiện không có hai thợ nào làm việc vào cuối tuần thành không có thợ nào làm việc vào cuối tuần.






