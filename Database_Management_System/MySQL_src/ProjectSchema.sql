#CREATE DATABASE CommunityIndex;
#Drop Database CommunityIndex;
USE CommunityIndex;
#DROP TABLE CITIZEN;
CREATE TABLE Citizen(
	SSN VARCHAR(10),
    FName VARCHAR(15),
    MName VARCHAR(15),
    LName VARCHAR(15),
    Gender VARCHAR(1),
    Birth_Date DATE,
    Death_Date DATE NULL DEFAULT NULL,
    Spouse_SSN VARCHAR(10) NULL DEFAULT NULL,
    Blood_Group VARCHAR(5),
    PRIMARY KEY (SSN)
    #FOREIGN KEY (Spouse_SSN) REFERENCES Citizen(SSN) ON DELETE CASCADE ON UPDATE CASCADE
);

#DROP TABLE organization;
CREATE TABLE Organization(
	Org_ID VARCHAR(10),
    Org_Name VARCHAR(45),
    Website VARCHAR(45),
	Phone_Number VARCHAR(15),
    Established_Date DATE,
    Closing_Date DATE NULL DEFAULT NULL,
    PRIMARY KEY (Org_ID)
);

#DROP TABLE employment_status;
CREATE TABLE Employment_Status(
	Citizen_SSN VARCHAR(10),
	Worker_ID VARCHAR(10),
	Org_ID VARCHAR(10),
    Employee_Position VARCHAR(50),
    Join_Date DATE,
    Leave_Date DATE,
    Profession VARCHAR(30),
    PRIMARY KEY (Citizen_SSN,Worker_ID,Org_ID),
    UNIQUE INDEX (Worker_ID,Org_ID),
    FOREIGN KEY (Citizen_SSN) REFERENCES Citizen(SSN) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (Org_ID) REFERENCES Organization(Org_ID) ON DELETE CASCADE ON UPDATE CASCADE
);

#DROP TABLE studied_in;
CREATE TABLE Studied_IN(
	Student_SSN VARCHAR(10),
    Student_ID VARCHAR(10),
    Institute_ID VARCHAR(10),
    Program_Name VARCHAR(15),
    Admission_Date DATE,
    Drop_Out_Date DATE,
	PRIMARY KEY (Student_ID, Institute_ID),
    FOREIGN KEY (Student_SSN) REFERENCES Citizen(SSN) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (Institute_ID) REFERENCES Organization(Org_ID) ON DELETE CASCADE ON UPDATE CASCADE
);

#DROP TABLE MEDICAL_RECORD;
CREATE TABLE Medical_Record(
	Patient_SSN VARCHAR(10),
    Record_ID VARCHAR(10),
    Medical_Issue VARCHAR(45),
    Medical_Status VARCHAR(15),
    Doctor_ID VARCHAR(10),
    Hosp_ID VARCHAR(10),
    Event_Date DATE,
    PRIMARY KEY (Patient_SSN,Record_ID),
    FOREIGN KEY (Patient_SSN) REFERENCES Citizen(SSN) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (Doctor_ID,Hosp_ID)
		REFERENCES Employment_Status(Worker_ID,Org_ID)
	ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE Marriage(
	Wife_SSN VARCHAR(10),
    Husband_SSN VARCHAR(10),
    Married_ON DATE,
    Divorced_ON DATE,
    PRIMARY KEY (Wife_SSN,Husband_SSN),
    FOREIGN KEY (Wife_SSN) REFERENCES Citizen(SSN) ON DELETE CASCADE ON UPDATE CASCADE,
    #Foreign key wrote in 2 different lines because it refers to same primary key of the referred table
    FOREIGN KEY (Husband_SSN) REFERENCES Citizen(SSN) ON DELETE CASCADE ON UPDATE CASCADE
);

#DROP TABLE Contact;
CREATE TABLE Contact(
	SSN VARCHAR(10),
    Email VARCHAR(45),
    Phone_Number VARCHAR(15),
    PRIMARY KEY (SSN,Email,Phone_Number),
    FOREIGN KEY (SSN) REFERENCES Citizen(SSN) ON DELETE CASCADE ON UPDATE CASCADE
);

#DROP TABLE FAMILY;
CREATE TABLE Family(
	Child_SSN VARCHAR(10),
    Parent_SSN VARCHAR(10),
    PRIMARY KEY (Child_SSN,Parent_SSN),
    FOREIGN KEY (Child_SSN) REFERENCES Citizen(SSN) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (Parent_SSN) REFERENCES Citizen(SSN) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE Property(
	Property_ID VARCHAR(10),
    Property_Type VARCHAR(15),
    Address TEXT,
    PRIMARY KEY (Property_ID)
);

#DROP TABLE OWNS;
CREATE TABLE Owns(
	Property_ID VARCHAR(10),
    Owner_SSN VARCHAR(10),
    PRIMARY KEY (Property_ID,Owner_SSN),
    FOREIGN KEY (Owner_SSN) REFERENCES Citizen(SSN) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (Property_ID) REFERENCES Property(Property_ID) ON DELETE CASCADE ON UPDATE CASCADE
);

#DROP TABLE Lives_IN;
CREATE TABLE Lives_IN(
	Property_ID VARCHAR(10) NOT NULL,
    Resident_SSN VARCHAR(10) NOT NULL,
    PRIMARY KEY (Resident_SSN),
    FOREIGN KEY (Resident_SSN) REFERENCES Citizen(SSN) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (Property_ID) REFERENCES Property(Property_ID) ON DELETE CASCADE ON UPDATE CASCADE
);

#DROP  TABLE CRIME;
CREATE TABLE Crime(
    Crime_ID VARCHAR(10),
	Criminal_Activity VARCHAR(45),
    Activity_Date DATE,
    Police_Station_Jurisdiction_ID VARCHAR(10),
    PRIMARY KEY (Crime_ID),
    FOREIGN KEY (Police_Station_Jurisdiction_ID) REFERENCES Organization(Org_ID) ON DELETE CASCADE ON UPDATE CASCADE
);

#DROP TABLE COMMITS;
CREATE TABLE Commits(
	Criminal_SSN VARCHAR(10),
    Crime_ID VARCHAR(10),
    Arrested_ON DATE NULL DEFAULT NULL,
    Released_ON DATE NULL DEFAULT NULL,
    PRIMARY KEY (Criminal_SSN,Crime_ID),
    FOREIGN KEY (Criminal_SSN) REFERENCES Citizen(SSN) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (Crime_ID) REFERENCES Crime(Crime_ID) ON DELETE CASCADE ON UPDATE CASCADE
);