CREATE TABLE TrafficSignal (
    Ticks INT,
    NSsign VARCHAR(255),
    WEsign VARCHAR(255),
    Switch BOOLEAN,
    switchTime INT,
    NeedChange BOOLEAN,
    NeedShow BOOLEAN,
    PERIOD INT,
    PRIMARY KEY (Ticks)
);
CREATE TABLE Cars (
    CarID INT AUTO_INCREMENT,
    RandomValue1 INT,
    RandomValue2 INT,
    PRIMARY KEY (CarID)
);
INSERT INTO TrafficSignal (Ticks, NSsign, WEsign, Switch, switchTime, NeedChange, NeedShow)
VALUES (0, 'RED', 'GREEN', FALSE, 5, FALSE, FALSE);
SELECT * from trafficLights where id = (SELECT max(id) FROM trafficLights);