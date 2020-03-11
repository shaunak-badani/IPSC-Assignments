# Assignment 3

### How to Run :

* Q5 - On Ada : 
    ```
    sinteractive -g 4
    module load cuda/10.2
    nvcc Q5.cu
    ./a.out <no of elements = optional argument>
    ```
    If No of elements is not provided 10^8 elements will be considered.