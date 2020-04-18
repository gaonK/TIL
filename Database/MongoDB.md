# MongoDB

> ☠️ 나를 울리는 MongoDB ☠️

MongoDB는 document 데이터베이스이다. JSON과 유사한 형태의 document로 data를 저장한다. 이는 관계형 데이터베이스들과 MongoDB가 차별화를 가지는 부분이라 할 수 있겠다. 왜 기존의 RDB가 아닌 이런 방식으로 저장할까? 이런 식의 DB는 좀 더 자연스러운 방식으로 객체를 표현할 수 있기 때문이다. MongoDB는 모던 어플리케이션을 위한 데이터베이스로 자신을 소개한다.



## 설치 및 실행

나는 회사에서도, 집에서도 mac을 사용하기 때문에 mac만 다룰 것이다. 이 문서에서 나는 버전 신경 쓰지 않는다! 특정 버전을 설치하고 싶다면 밑에 있는 내용에서 mongodb-community@`version` 이런 식으로 버전을 표시해주면 된다. ([#](https://docs.mongodb.com/manual/tutorial/install-mongodb-on-os-x/))

brew tap을 이용해서 설치해야 한다. 좀 오래된 문서에서는 brew install mongodb로 가이드하지만 더 이상 그렇게는 설치할 수 없다.

```
$ brew tap mongodb/brew
$ brew install mongodb-community
$ brew services start mongodb-community	
```

mongo shell을 실행시키고 싶다면 이렇게 치자.

```
$ mongo
```

so simple. 

로컬에 있는 특정 주소의 데이터베이스를 실행시키고 싶다면?

```
$ mongo mongodb://localhost/이름
```



## Document Database

MongoDB의 가장 두드러지는 특징이라고 한다면 Document Database가 아닐까? MongoDB의 document는 JSON 객체와 많이 닮았다. Document에서는 key-value pair라고 하는 대신 field-value pair라고 한다는 것을 알아두자.

* Documents는 많은 언어에서 실제 데이터 타입과 일치한다.
* 객체 내부에 다른 객체나 배열을 둬야 하는 경우 RDB에서와는 달리 join 연산을 줄일 수 있다.
* 동적 스키마가 지원되서 fluent polymorphism, 즉 다형성을 지원한다. RDB와는 달리 스키마를 명확하게 명시하지 않아도 되기 때문에 동적으로 스키마를 변경시킬 수 있다.

### Collection

MongoDB의 document들은 collection에 저장이 된다. Collection은 RDB에서의 테이블이라고 볼 수 있겠다. 또한 이러한 collection들에 대해 RDB에서와 마찬가지로 view를 구성할 수 있게 할 수도 있다. 



## 기본적인 CRUD

### Create Operations

* [`db.collection.insertOne()`](https://docs.mongodb.com/manual/reference/method/db.collection.insertOne/#db.collection.insertOne)
* [`db.collection.insertMany()`](https://docs.mongodb.com/manual/reference/method/db.collection.insertMany/#db.collection.insertMany)

### Read Operations

* [`db.collection.find()`](https://docs.mongodb.com/manual/reference/method/db.collection.find/#db.collection.find)

### Update Operations

* [`db.collection.updateOne()`](https://docs.mongodb.com/manual/reference/method/db.collection.updateOne/#db.collection.updateOne)
* [`db.collection.updateMany()`](https://docs.mongodb.com/manual/reference/method/db.collection.updateMany/#db.collection.updateMany)
* [`db.collection.replaceOne()`](https://docs.mongodb.com/manual/reference/method/db.collection.replaceOne/#db.collection.replaceOne)

### Delete Operations

* [`db.collection.deleteOne()`](https://docs.mongodb.com/manual/reference/method/db.collection.deleteOne/#db.collection.deleteOne) 
* [`db.collection.deleteMany()`](https://docs.mongodb.com/manual/reference/method/db.collection.deleteMany/#db.collection.deleteMany) 



## 데이터 dump, restore

https://docs.mongodb.com/manual/tutorial/backup-and-restore-tools/

위의 문서에서는 예시가 '=' 기호를 이용하고 있는데 실제로는 안 들어가는 게 맞다. 현재 돌아가고 있는 MongoDB 데이터를 백업하고 복원시킬 때 사용한다. 나 같은 경우는 이번에 로컬에서 테스트를 하기 위해 실제 데이터를 dump 후 localhost에 restore 해서 작성한 스크립트가 원하는 대로 작동하는지 테스트했다.

### dump 예시

```
mongodump --host mongodb1.example.net --port 3017 --username user --password pass --out /opt/backup/mongodump-2013-10-24
```

### restore 예시

```
mongorestore --host mongodb1.example.net --port 3017 --username user  --authenticationDatabase admin /opt/backup/mongodump-2013-10-24
```

