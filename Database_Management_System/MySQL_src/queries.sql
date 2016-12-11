USE CommunityIndex;

#Query-1 Most aged Citizens in Community.(First 5)
SELECT SSN,FName,DATEDIFF(CURDATE(),Birth_Date)/365 AS AGE
	FROM Citizen
ORDER BY AGE DESC LIMIT 5;

#Query-2 Least aged Citizens in Community.(First 5)
SELECT SSN,FName,DATEDIFF(CURDATE(),Birth_Date)/365 AS AGE
	FROM Citizen
ORDER BY AGE ASC LIMIT 5;

#Query-3 Total no. of people having surname 'Patel', 'Mehta', 'Parikh', 'Sanghavi'
SELECT LName, COUNT(LName) AS SurnameCount
	FROM Citizen WHERE LName = 'Patel' OR LName = 'Mehta' OR LName = 'Parikh' OR LName = 'Sanghavi'
GROUP BY LName;

#Query-4 Largest Group in the Community
SELECT LName, COUNT(LName) AS SurnameCount
	FROM Citizen
GROUP BY LName
ORDER BY SurnameCount DESC LIMIT 1;

#Query-5 Total no. of Married Couples
SELECT COUNT(Spouse_SSN) AS CoupleCount
	FROM Citizen
WHERE Spouse_SSN IS NOT NULL;

#Query-6 No. of Males and Females in the Community
SELECT Gender, COUNT(SSN) AS GenderCount
	FROM Citizen
GROUP BY Gender;

#Query-7 Citizens who have committed more than 1 crime
SELECT c1.Criminal_SSN, c2.FName, COUNT(c1.Crime_ID) AS NoOfCrimes
FROM Commits c1 JOIN
	 Citizen c2 ON
		c1.Criminal_SSN=c2.SSN
GROUP BY c1.Criminal_SSN HAVING NoOfCrimes>1;

#Query-8 Criminal Citizens who are not yet released
SELECT c1.Criminal_SSN, c2.FName, COUNT(c1.Crime_ID) AS NoOfCrimes
FROM Commits c1 JOIN
	 Citizen c2 ON
		c1.Criminal_SSN=c2.SSN
WHERE c1.Released_ON IS NULL AND c1.Arrested_ON IS NOT NULL
GROUP BY c1.Criminal_SSN;

#Query-9 Criminal Citizens who are absconding
SELECT c1.Criminal_SSN, c2.FName, COUNT(c1.Crime_ID) AS NoOfCrimes
FROM Commits c1 JOIN
	 Citizen c2 ON
		c1.Criminal_SSN=c2.SSN
WHERE c1.Arrested_ON IS NULL
GROUP BY c1.Criminal_SSN;

#Query-10 Top 5 Most no. of Crimes Committed
SELECT Criminal_Activity, COUNT(Crime_ID) AS noOfTimes
	FROM Crime 
GROUP BY Criminal_Activity
ORDER BY noOfTimes DESC LIMIT 5;

#Query-11 No. of persons having phone no. but no email
SELECT COUNT(DISTINCT SSN) AS CountValue
	FROM Contact
WHERE Email='NA';

#Query-12 Average AGE of Community
SELECT AVG(DATEDIFF(CURDATE(),Birth_Date))/365 AS AverageAge
	FROM Citizen
WHERE Death_Date IS NULL;

#Query-13 Total Senior Citizens in Community
SELECT COUNT(SSN) AS CountValue
	FROM Citizen
WHERE DATEDIFF(CURDATE(),Birth_Date)/365>58;

#Query-14 No. of SSN in respetive Blood Groups 
SELECT Blood_Group, COUNT(SSN) AS Count_Value
	FROM Citizen
GROUP BY Blood_Group;

#Query-15 Top 3 Police-Stations where crimes are registered
SELECT Police_Station_Jurisdiction_ID, COUNT(Crime_ID) AS CrimeCount
	FROM Crime
GROUP BY Police_Station_Jurisdiction_ID
ORDER BY CrimeCount DESC LIMIT 1;

#Query-16 Oldest Organization
SELECT Org_ID, Org_Name, DATEDIFF(CURDATE(),Established_Date)/365 AS Years_Old
	FROM Organization
ORDER BY Years_Old DESC LIMIT 1;

#Query-17 More than 1 owners of a property
SELECT Property_ID,COUNT(Owner_SSN) AS noOfOwners
	FROM Owns 
GROUP BY Property_ID HAVING noOfOwners>1;

#Query-18 No. of Commercial, 4-BHK, 3-BHK Apartments
SELECT Property_Type, COUNT(Property_ID) AS Property_Count
	FROM Property
GROUP BY Property_Type;

#Query-19 Area-wise Property count
SELECT Address, COUNT(Property_ID) AS PropertyCount 
	FROM Property
GROUP BY Address;

#Query-20 No. of Properties, name owned by individual Ministers
SELECT c.Fname AS Name, o.owner_ssn AS SSN, count(o.Property_ID) AS PropertyCount
	FROM owns AS o
    JOIN employment_status AS e
		ON o.owner_ssn=e.citizen_ssn
	JOIN citizen as c
		on c.ssn = o.owner_ssn
WHERE e.Profession = 'Minister'
GROUP BY o.owner_ssn
ORDER BY PropertyCount DESC;

#Query-21  Dengues in Monsoon Season
SELECT Medical_Issue, Count(record_id)
	from medical_record
WHERE month(Event_Date)>=6 and month(Event_Date)<=9 and Medical_Issue='Dengue'
GROUP BY Medical_Issue;

#Query-22 total 10th, 12th, engineers,etc. in the community
SELECT Program_Name, count(Student_SSN) as StudentCount 
	FROM studied_in
GROUP BY Program_Name
ORDER BY StudentCount desc;

#Query-23 Citizens changing job/work in less than an year
SELECT DISTINCT c.Fname as Name, e.Citizen_SSN as SSN
	FROM employment_status AS e
    JOIN citizen AS c
		ON c.ssn = e.Citizen_SSN
WHERE datediff(e.Leave_Date,e.Join_Date)<365;

#Query-24 Family having more than 1 child
SELECT f.Parent_SSN as SSN, c1.fname as Name, count(f.Child_SSN) as childrenCount
	FROM citizen as c1
    JOIN family as f
		ON c1.ssn = f.Parent_SSN
	JOIN citizen as c2
		ON c2.ssn = f.Child_SSN
WHERE datediff(curdate(),c2.Birth_Date)/365<18
GROUP BY f.Parent_SSN
HAVING childrenCount>1;

#Query-25 Cousins
SELECT f2.child_ssn as child1, f4.child_ssn as Child2
	FROM family as f1 #parent = grandparent
    JOIN family as f2 #parent = father
		ON f1.child_ssn = f2.parent_ssn 
	JOIN family as f3
		ON f1.parent_ssn = f3.parent_ssn AND
		   f1.child_ssn != f3.child_ssn
	JOIN family as f4
		ON f4.parent_ssn = f3.child_ssn;

#Query-26 Same organization - school
SELECT a.Student_SSN as Citizen1,b.Student_SSN as Citizen2
	FROM
	(SELECT s1.Institute_ID,e1.org_ID,s1.Student_SSN
		FROM studied_in s1
		JOIN employment_status e1
			ON s1.student_ssn=e1.Citizen_SSN WHERE s1.Institute_ID!=e1.Org_ID) as a
	JOIN
	(SELECT s2.Institute_ID,e2.org_ID,s2.Student_SSN
		FROM studied_in s2
		JOIN employment_status e2
			ON s2.student_ssn=e2.Citizen_SSN WHERE s2.Institute_ID!=e2.Org_ID) as b
	ON (a.Institute_ID=b.Institute_ID AND a.Org_ID=b.org_ID)
	WHERE a.student_ssn!=b.student_ssn AND a.student_ssn<b.student_ssn;

#Query-27 Govt Officials having more than 1 property, and lives in Res2BHK
SELECT e.Citizen_SSN as GovtOfficialSSN,count(o1.Property_ID) as propertyCount
	FROM employment_status e
	JOIN owns o1
		ON e.Citizen_SSN=o1.Owner_SSN
	WHERE e.profession='Government Official'
	  AND e.Citizen_SSN IN
      (SELECT o2.Owner_SSN
			FROM owns o2
			JOIN property p
				ON p.Property_ID=o2.Property_ID
			WHERE p.Property_Type='Res2BHK')
GROUP BY e.Citizen_SSN
HAVING propertyCount>1;

#Query-28 Age above 60 still employed
SELECT e.Citizen_SSN as SSN,c.Fname as Name,datediff(curdate(),c.Birth_Date)/365 as AGE
FROM employment_status e
	JOIN Citizen c
		ON e.Citizen_SSN=c.SSN
#WHERE e.Leave_Date IS NULL
HAVING AGE>60;

#Query-29 Citizens still employed after having heart attack or cancer
SELECT e.Citizen_SSN as SSN,c.Fname as Name
FROM employment_status e
	JOIN Citizen c
		ON e.Citizen_SSN=c.SSN
	JOIN medical_record m
		ON m.Patient_SSN=c.SSN
WHERE e.Leave_Date IS NULL AND (m.medical_issue='HeartAttack' OR m.medical_issue='Cancer');

#Query-30 Couple Married for more than 3 years and not having a child
SELECT m1.Husband_SSN,m1.Wife_SSN,datediff(curdate(),m1.Married_ON)/365 as MarriedYears
FROM marriage m1
	WHERE m1.Husband_SSN NOT IN
    (SELECT f.Parent_SSN
	 FROM family f) AND m1.Divorced_ON IS NULL
HAVING MarriedYears>=10
order by MarriedYears DESC;