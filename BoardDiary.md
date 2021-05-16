-Game Of Life, MPI Implementation

-Name: Gilberto Recupito 

-Github: gilbertrec


#First Problem: Matrix Suddivision
This problem presents the division of the matrix for the p processes
We have different ways to divide the matrix, some are:

-by rows
-by columns
-by submatrices

for the first two suddivision techniques we have to consider that the first and the last row of each partition must be sent to the previous and the next process for each generation, having so 2P messages for each process(P)

for the third suddivision techniques(by submatrices) we have to consider that for each cell in the border we can need to receive up to 3 messages from 3 different process, so in some cases (for example if a process get a submatrices in the center) must send up to 4 messages.
However, in better cases (for example a submatrices in the corner) we have to send two messages.

for this reason we chose one of the first two techniques.

And now, which of the following two, columns or rows?

We have to consider first the size of the matrix,
Let's consider S a matrix of dimension n*m

If we consider that the number of processes p is equal to m, and we divide the matrix by m, we have that each processes has one line of the matrix
This means that for each element that must be elaborated, the process need to receive from the other 2 processes(left and right, or in case of columns above and below) all the line messages.

In the other side if we have a great number of process p and p >>m (much greater than),and we divide the matrix by m, we have that each processes has a great number of cells each one, and the internal cells doesn't need communication in order to be computated.

So, after this consideration we decided to divide the matrix in the smaller dimension between rows and columns.
