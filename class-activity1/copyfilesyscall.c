#include <unistd.h>
#include <fcntl.h>

int main() {
    int src, dest;
    char buffer[1024];
    ssize_t bytesRead;

    // Open source file (read only)
    src = open("result.txt", O_RDONLY);

    // Create destination file (write only)
    dest = open("copyresult.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    // Copy loop
    while ((bytesRead = read(src, buffer, sizeof(buffer))) > 0) {
        write(dest, buffer, bytesRead);
    }

    // Close files
    close(src);
    close(dest);

    return 0;
}
