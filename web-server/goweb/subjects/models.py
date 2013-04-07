from django.db import models

# Create your models here.
class subject(models.Model):
	name = models.CharField(max_length=30)
	desc = models.CharField(max_length=256)
	

class article(models.Model):
	title = models.CharField(max_length=128)
	uri = models.URLField()
	content = models.CharField()

class comment(models.Model):
	to_user = models.IntField()

class user(models.Model):
	name = models.CharField(max_length=64)	
