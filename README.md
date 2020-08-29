# TechAtFarm
Water supply and fertilizer recommendation system

#### App-type:
  - Web application.
  - Conversion to Progressive Web App (PWA) for mobile/tablet devices if required.
  - Android Studio / Flutter not yet involved.

#### Backend:
  - Framework: Django 2 (Python) application server
  - Database : SQLite (relational DBMS - Django's default)

#### Frontend:
  - HTML/CSS/Bootstrap and JavaScript/jQuery (although template based).
  - Not using React (JS/Native) yet.

#### Deployment:
  - Web Server: Amazon Web Services

#### Overview:
  This project is inspired by the problem statement given by the Aqua App (TM). However, we do not follow the actual structure of the original app. Thus, some functionalities have been added.

  - We aim to develop an app for agricultural sector, capable of managing water (+ fertilizers) supply and distributing - it to farms / other facilities of India.
  - It maintains a datbase of customers (farmers) and operators who control the app to serve the customer. An admin's login is also provided.
  - The features available to different user types will vary as they login with their credentials.
  - The app analyzes the land quality, weather, crop type(s), soil nutrients, humidity, moisture, etc. and recommends the best fertilizers and adequate water quantity.
  - The analysis can be done by a surveyor who goes with his equipments and conducts tests and measurements of land size etc., creates an ID for farmers and feeds the data to the database.
  - The app also controls the water supply and calculates the bill accordingly.
  - This requires development and integration of IoT (Internet of Things) devices with the app and the motors supplying water.

#### Additional Functionalities:
  - A recommendation system based on climate (sunlight, rainfall, humidity, etc.) data to predict the best crop type(s) suitable for the season.
  - A mobile messaging and notification system telling farmers about daily weather updates and market prices of the crops they grow.
  - A blog section where experts write about informed methods of farming and recommending practices that require less investment and generate more revenue.
  - The farmers can even declare the crops and the prices at which they want to sell upon login, either by themselves or by seeking help of the regional app managers.

####  Modules / Add-ons / Sub-apps:
  To be announced soon.

#### Project Outline:
https://docs.google.com/document/d/1X-7tY8D8jl82GbZbgbnCYqHKnwGCr8rn5XT41gr71PQ/edit

#### Installation

Installing the dependencies and starting the server.
  - git
  - Python3 (>= 3.5.x)
  - pip3 (>= 19.x.x)
  - virtualenv (recommended to install dependencies in a virtual environment)
  - Django (>= 2.2.3).

```bash
$ git clone https://github.com/Ishanh2000/TechAtFarm
$ virtualenv venv
$ source venv/bin/activate
$ pip3 install Django
$ cd TechAtFarm
$ python3 manage.py makemigrations
$ python3 manage.py migrate
$ python3 manage.py runserver
```

For deployment on [AWS](https://aws.amazon.com)...
  - Create an AWS instance. Make sure that your rule allows incoming request via port 80 (change if you want to).
  - Connect to the instance. Install dependencies just like demonstrated above.

```bash
$ export PORT=80
$ python3 manage.py runserver 0.0.0.0:$PORT
```

Check if your site is online by typing IP address of your instance in the browser address bar.

#### Team Name: SoC PANDAVAS

#### Team Members:
  - Aditya Dikshit ([imdikshit](https://github.com/imdikshit))
  - Dibyojyoti Sinha ([DibyojyotiS](https://github.com/DibyojyotiS))
  - Ishanh Misra ([Ishanh2000](https://github.com/Ishanh2000))
  - Shubham Gupta ([shubhamgupta1313](https://github.com/shubhamgupta1313))
  - Yash Vardhan Raizada ([yashvardhanraizada](https://github.com/yashvardhanraizada))

#### Side Note:
  - This project was developed during the Summer of Code (SoC) 2019 at IIT Kanpur as a part of a competition.
  - The team secured the first position in this competition.
  - [SoC 2019](https://soc.cse.iitk.ac.in) was hosted by the CSE dept. of IIT Kanpur and sponsored by Nutanix and UPSIDC.

#### TODO:
  - Update "Modules / Add-ons / Sub-apps" and "Project Outline" sections.
  - Create GitHub page for this repo.
