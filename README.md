Một bản sao trờ chơi Sudoku được làm bằng C/C++ và các thư viện SDL2.

Dự án chỉ hỗ trợ tiếng Anh.

Để build và chạy, hãy sao chép repository này bằng Visual Studio 2022. Các thư viện SDL2 đã được thêm vào trong repository.

Các chức năng chính:

- Lưu và tải file màn chơi
- Chọn độ khó: dễ, trung bình và khó
- Giải câu đó Sudoku
- Kiểm tra tính hợp lệ

Lưu ý

- Trong giao diện tải file, nếu có hơn 5 file thì xử dụng phìm mũi tên lên hoặc xuống để cuộn lên hoặc xuống. Trong thư mục Saves đã có một vài file được lưu.
  
- Chức năng chơi sẽ tải file trong trong thư mục Levels, trong này sẽ có 3 thư mục là easy, medium và hard, nếu bạn thêm file màn chơi khác vào thì hãy vào
file Levemanagers trong thư mục src và thay đổi hàm setRandomLevel để nó có thể sự dụng thêm file. Mặc định nó sẽ lấy ngẫu nhiên 1 trong số 5 file được lưu.
LƯU Ý: Hàm nay sử dụng cho cả 3 mức độ nên khi thay đổi hãy đảm bảo rằng trong các thư mục easy, medium và hard có số file màn chơi tương tự nhau.

                int LevelsManager::setRandomLevel() {
                	std::random_device rd;
                	std::mt19937 gen(rd());
                	std::uniform_int_distribution<> dis(1, 5); //Thay đổi giá trị 5 để nó lấy thêm file
                
                	int randomLevel = dis(gen);
                	return randomLevel;
                } 
  
---------------------------------------------------------------------------------------------------------------------------------

A Sudoku clone made in C/C++ with SDL2 libraries.

The project only supports English.

To build and run, clone this repository with Visual Studio 2022. SDL2 libraries have been included in the repository.

Main functions:

- Save and load game files
- Choose difficulty: easy, medium, and hard
- Solve the Sudoku puzzle
- Check validity


Note

- In the file upload interface, if there are more than 5 files, use the up or down arrow keys to scroll up or down. In the Saves folder, there are already a few files saved.

- The gameplay function will load files from the Levels folder, which contains three subfolders: easy, medium, and hard. If you add another level file, go to the Levemanagers file in the src folder and modify the setRandomLevel function so it can use the additional file. By default, it will randomly select one of the five stored files.
NOTE: This function is used for all 3 levels, so when making changes, please ensure that the number of game files in the easy, medium, and hard folders is similar.

                int LevelsManager::setRandomLevel() {
                	std::random_device rd;
                	std::mt19937 gen(rd());
                	std::uniform_int_distribution<> dis(1, 5); //Change the value to 5 so it can take more files.
                
                	int randomLevel = dis(gen);
                	return randomLevel;
                } 
