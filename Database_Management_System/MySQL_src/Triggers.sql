delimiter //
create trigger date_violation
	before insert on Citizen
	for each row
begin
	declare msg varchar(128);
    if(datediff(new.Death_Date,new.Birth_Date)<0) then
		set msg = "Date error";
        signal sqlstate '45000' set message_text = msg;
	end if;
end
//
delimiter ;

delimiter //
create trigger employee_joindate_violation
	before insert on employment_status
	for each row
begin
	declare msg varchar(128);
	
     set @X = (SELECT Established_Date FROM Organization WHERE Org_ID=new.Org_ID);
    if datediff(new.Join_Date,@X)<0 and new.Org_ID is not null then
		set msg = "Organization not formed";
        signal sqlstate '45000' set message_text = msg;
	end if;
end
//
delimiter ;