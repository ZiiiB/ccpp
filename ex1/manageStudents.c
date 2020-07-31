/**
* Created by ziv on 23/03/2020.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// optional inputs for program
#define BEST "best"
#define MERGE "merge"
#define QUICK "quick"

#define NEWLINE "\n"
#define QUIT_WITH_NEWLINE "q\n"

// parameters for the validation of params
#define HYPHAN 45
#define ZERO 48
#define WHITESPACE 32
#define MIN_GRADE 0
#define MAX_GRADE 100
#define MIN_AGE 18
#define MAX_AGE 120
#define WITH_SPACE 1
#define NOSPACE 0
#define ID_LEN 10

// parameters for size of array
#define MAX_LEN_LINE 64
#define MAX_LEN_SINGLE_DATA 42
#define MAX_NUM_OF_STUDENTS 5500


// a student struct to save all the data of each student
struct Student
{
	int age;
	int grade;
	char ID[ID_LEN + 1];
	char name[MAX_LEN_SINGLE_DATA + 1];
	char city[MAX_LEN_SINGLE_DATA + 1];
	char country[MAX_LEN_SINGLE_DATA + 1];
};

typedef struct Student student;


/**
 *
 * @param word to check if valid
 * @param withSpace int to check spaces
 * @return 0 if valid 1 if not
 */
int notValidWord(const char *word, const int withSpace)
{
	if (word[0] == 0)
	{
		return 1;
	}
	int i = 0;
	while (word[i])
	{
		if (!isalpha(word[i]) && word[i] != HYPHAN)
		{
			if (word[i] == WHITESPACE)
			{
				if (!withSpace)
				{
					return 1;
				}
			}
			else
			{
				return 1;
			}
		}
		i++;
	}
	return 0;
}


/**
 * check if ID is 10 digit long and contains
 * only digits that the first is not zero
 * @param ID string of ID
 * @return 1 if not valid 0 else
 */
int notValidID(const char *ID)
{
	if (ID[0] == ZERO)
	{
		return 1;
	}
	int i = 0;
	while (ID[i])
	{
		if (!isdigit(ID[i]) || i >= ID_LEN)
		{
			return 1;
		}
		i++;
	}
	if (i < ID_LEN)
	{
		return 1;
	}
	return 0;
}


/**
 * if not valid prints error message
 * @param s student
 * @param i number of line
 * @return 1 if student is valid 0 if not
 */
int valid(const student s, const int i)
{
	// check validity of ID
	if (notValidID(s.ID))
	{
		printf("ERROR: ID should contain 10 digits, the first digit should not be 0\n");
		printf("in line %d\n", i);
		return 0;
	}
	if (notValidWord(s.name, WITH_SPACE))
	{
		printf("ERROR: name contains only alphabetic characters or -\nin line %d\n", i);
		return 0;
	}
	if (s.grade > MAX_GRADE || s.grade < MIN_GRADE)
	{
		printf("ERROR: grade should be between 0 and 100\nin line %d\n", i);
		return 0;
	}
	if ((s.age < MIN_AGE) || (s.age > MAX_AGE))
	{
		printf("ERROR: age %d should be between 18 to 120\nin line %d\n", s.age, i);
		return 0;
	}
	if (notValidWord(s.country, NOSPACE))
	{
		printf("ERROR: country contains only alphabetic characters or - without whitespaces\n");
		printf("in line %d\n", i);
		return 0;
	}
	if (notValidWord(s.city, NOSPACE))
	{
		printf("ERROR: city contains only alphabetic characters or - without whitespaces\n");
		printf("in line %d\n", i);
		return 0;
	}
	return 1;
}


/**
 * check if all char in string are digit
 * @param string
 * @return 1 if not 0 else
 */
int isNotDigit(const char *string)
{
	if (string[0] == ZERO && string[1] != 0)
	{
		return 1;
	}
	int i = 0;
	while (string[i])
	{
		if (!isdigit(string[i]))
		{
			return 1;
		}
		i++;
	}
	return 0;
}


/**
 * get strings for initializing a student struct
 * and initialize student struct
 * @param ID string of ID
 * @param name string of name
 * @param grade string of name
 * @param age string of age
 * @param country string of country
 * @param city string of city
 * @param Student student struct
 */
void stringsToStudents(const char *ID, const char *name, const char *grade, const char *age,
					   const char *country, const char *city, student *Student)
{
	Student->age = (int) strtol(age, NULL, 10);
	Student->grade = (int) strtol(grade, NULL, 10);
	strcpy(Student->ID, ID);
	strcpy(Student->name, name);
	strcpy(Student->country, country);
	strcpy(Student->city, city);
}

/**
 * get in a loop input from the user
 * @param students students struct list
 * @return number of valid values in students array
 */
int getStudentsData(student *students)
{
	char inputString[MAX_LEN_LINE];
	int i = 0;
	int idx = 0;
	// while not quit or max number of students
	printf("Enter student info. To exit press q, then enter\n");
	while (strcmp(fgets(inputString, MAX_LEN_LINE, stdin), QUIT_WITH_NEWLINE) &&
		   idx < MAX_NUM_OF_STUDENTS)
	{
		char ID[MAX_LEN_SINGLE_DATA + 1] = "", name[MAX_LEN_SINGLE_DATA + 1] = "",
				country[MAX_LEN_SINGLE_DATA + 1] = "", city[MAX_LEN_SINGLE_DATA + 1] = "",
				grade[MAX_LEN_SINGLE_DATA + 1] = "", age[MAX_LEN_SINGLE_DATA + 1] = "";

		sscanf(inputString, "%[^,\n],%[^,\n],%[^,\n],%[^,\n],%[^,\n],%[^,\n]",
			   ID, name, grade, age, country, city);

		if (isNotDigit(age) || isNotDigit(grade))
		{
			printf("ERROR: age and grade should be digits only.\n");
			printf("in line %d\n", i);
		}
		else
		{
			stringsToStudents(ID, name, grade, age, country, city, &(students[idx]));

			if (valid(students[idx], i))
			{
				idx++;
			}
		}
		printf("Enter student info. To exit press q, then enter\n");
		i++;
	}
	return idx;
}


/**
 * checks which students has the best info
 * and prints his info to the user
 * @param students list
 */
void returnBestToUser(const student *students, const int len)
{
	if (len <= 0)
	{
		return;
	}
	student best = students[0];
	for (int idx = 1; idx < len; idx++)
	{
		if (students[idx].grade * best.age > students[idx].age * best.grade)
		{
			best.age = students[idx].age;
			best.grade = students[idx].grade;
			strcpy(best.ID, students[idx].ID);
			strcpy(best.name, students[idx].name);
			strcpy(best.country, students[idx].country);
			strcpy(best.city, students[idx].city);
		}
		else
		{
			if (students[idx].grade * best.age == students[idx].age * best.grade &&
				best.age > students[idx].age)
			{
				best.age = students[idx].age;
				best.grade = students[idx].grade;
				strcpy(best.ID, students[idx].ID);
				strcpy(best.name, students[idx].name);
				strcpy(best.country, students[idx].country);
				strcpy(best.city, students[idx].city);
			}
		}
	}
	printf("best student info is: %s,%s,%d,%d,%s,%s\n", best.ID, best.name, best.grade,
		   best.age, best.country, best.city);
	return;
}


/**
 * merge two parts of array which are ordered
 * @param StudentsArr array with the values of the students
 * the array indicesArr contains the indexes of the these array
 * @param indicesArr array which contain the values of indexes to sort
 * @param start index of the first half of the array to merge
 * @param middle index of the secound half of the array to merge
 * @param end index of the end of the part to merge in the array
 */
void merge(const student *StudentsArr, int *indicesArr, int start, int middle, int end)
{
	int i, j, k;
	// the len of each halve of the array
	int n1 = middle - start + 1;
	int n2 = end - middle;

	// create two tmp arrays to merge
	int L[MAX_NUM_OF_STUDENTS], R[MAX_NUM_OF_STUDENTS];
	for (i = 0; i < n1; i++)
	{
		L[i] = indicesArr[start + i];
	}
	for (j = 0; j < n2; j++)
	{
		R[j] = indicesArr[middle + 1 + j];
	}

	// Merge the temp arrays back
	i = 0;
	j = 0;
	k = start;
	while (i < n1 && j < n2)
	{
		// compare between the values of the two places
		// (not indexes which are the values of the array)
		if (StudentsArr[L[i]].grade <= StudentsArr[R[j]].grade)
		{
			indicesArr[k] = L[i];
			i++;
		}
		else
		{
			indicesArr[k] = R[j];
			j++;
		}
		k++;
	}

	// copy the remaining
	while (i < n1)
	{
		indicesArr[k] = L[i];
		i++;
		k++;
	}
	while (j < n2)
	{
		indicesArr[k] = R[j];
		j++;
		k++;
	}
}


/**
 * merge sort as thought in data structure course
 * using the merge function (as explained in DAST course)
 * @param StudentsArr array of students to sort the indexes array
 * these array of students gives the values to compare on
 * @param indicesArr array of indexes to of students to compare
 * @param start index in indicesArr to start the algorithm
 * start at 0 for the first call and important
 * for the recursive part
 * @param end index in indicesArr to end the algorithm
 * start at 0 for the first call and important
 * for the recursive part
 */
void mergeSortOnIndices(const student *StudentsArr, int *indicesArr, int start, int end)
{
	if (start < end)
	{
		int middle = (end + start) / 2;

		// Sort first and second halves
		mergeSortOnIndices(StudentsArr, indicesArr, start, middle);
		mergeSortOnIndices(StudentsArr, indicesArr, middle + 1, end);
		//merge the two halves
		merge(StudentsArr, indicesArr, start, middle, end);
	}
}


/**
 * a utility to swap between the place in the memory of two values
 * @param pointer1 pointer to value1
 * @param pointer2 pointer to value2
 */
void swap(int *pointer1, int *pointer2)
{
	// switch values
	int tmp = *pointer1;
	*pointer1 = *pointer2;
	*pointer2 = tmp;
}


/**
 * do a partition of the slice array between start to end
 * the pivot of the partition is the end
 * @param StudentArr array of student structs
 * with info on students
 * @param indicesArr array of indices to compare
 * the values of these indices in the students array
 * @param start the first index in the array to refer to
 * @param end last value in the array to refer to
 * @return the new index of the pivot
 */
int partition(const student *StudentArr, int *indicesArr, int start, int end)
{
	int i = start;

	for (int j = start; j <= end - 1; j++)
	{
		// compare between two name if the first is first alphabetically
		// then strcmp will return a negative number
		if (strcmp(StudentArr[indicesArr[j]].name, StudentArr[indicesArr[end]].name) < 0)
		{
			swap(&indicesArr[i], &indicesArr[j]);
			i++;
		}
	}
	swap(&indicesArr[i], &indicesArr[end]);
	return i;
}


/**
 * sortes the indices array indicesArr by the values of the names in StudentsArr
 * sorts with the quick sort algorithm using a partition function
 * @param StudentsArr array of students struct with the values of the students info
 * @param indicesArr array of indices of each students
 * will be sorted via the values of StudentsArr
 * @param start start index to start the algorithm on a part of the array indicesArr
 * in first recursive itter will be 0
 * @param end last index of the array who take part in the sort
 * in first itter of the algorithm will be the index of the last elemment in the list
 */
void quickSortOnIndices(const student *StudentsArr, int *indicesArr, int start, int end)
{
	if (start < end)
	{
		//these middle point is not necessary in the middle
		int middle = partition(StudentsArr, indicesArr, start, end);

		quickSortOnIndices(StudentsArr, indicesArr, start, middle - 1);
		quickSortOnIndices(StudentsArr, indicesArr, middle + 1, end);
	}
}


/**
 * running quick/ merge sort on the array of indices as explained in the function
 * and then printing the sorted students info
 * @param students array of student struct with the info of the students
 * @param len length of the valid part of the students array
 * @param SortType sort type (quick by names or merge by grades)
 */
void returnSortToUser(const student *students, const int len, char *SortType)
{
	if (len <= 0)
	{
		return;
	}
	// create array of indices of the students array
	int indicesArr[MAX_NUM_OF_STUDENTS];
	for (int i = 0; i < len; i++)
	{
		indicesArr[i] = i;
	}

	if (!strcmp(SortType, MERGE))
	{
		// do merge sort on the indices array referring to the students array
		// grades
		mergeSortOnIndices(students, indicesArr, 0, len - 1);
	}
	else
	{
		// do quick sort on the indices array referring to the students array
		// names
		quickSortOnIndices(students, indicesArr, 0, len - 1);
	}
	// print to the user all the students info sorted
	for (int i = 0; i < len; i++)
	{
		printf("%s,%s,%d,%d,%s,%s\n", students[indicesArr[i]].ID, students[indicesArr[i]].name,
			   students[indicesArr[i]].grade, students[indicesArr[i]].age,
			   students[indicesArr[i]].country, students[indicesArr[i]].city);
	}
	return;
}


/**
 * main function of the file manageStudents
 * @param argc int number of args in argv
 * @param argv run params (should contain 2 elements)
 * @return 0 if the program ran successfully else 1
 */
int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("USAGE: Wrong number of args. abort.\n");
		return 1;
	}

	if (strcmp(argv[1], BEST) != 0 && strcmp(argv[1], MERGE) != 0 && strcmp(argv[1], QUICK) != 0)
	{
		printf("USAGE: input should be %s, %s or %s only\n", BEST, MERGE, QUICK);
		return 1;
	}

	student studentsArr[MAX_NUM_OF_STUDENTS];

	int len = getStudentsData(studentsArr);

	if (!strcmp(argv[1], BEST))
	{
		returnBestToUser(studentsArr, len);
	}
	else if (!strcmp(argv[1], MERGE) || !strcmp(argv[1], QUICK))
	{
		returnSortToUser(studentsArr, len, argv[1]);
	}
	return 0;
}

