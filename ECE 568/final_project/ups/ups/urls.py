"""ups URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/3.0/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path,include
from django.contrib.auth import views as auth_views
from django.views.generic.base import TemplateView

from user import views as user_views
from package import views as package_views

urlpatterns = [
    # path('admin/', admin.site.urls),
    # path('login/', auth_views.LoginView.as_view(template_name = 'user/login.html'), name = 'login'),
    # path('logout/', auth_views.LogoutView.as_view(template_name = 'user/logout.html'), name = 'logout'),
    # path('register/', auth_views.LogoutView.as_view(template_name = 'user/register.html'), name = 'register'),
    # path('track_package', package_views.track_package, name = 'track-package'),

    path('package/', include('package.urls')),
    path('user/', include('user.urls')),
    # path('', TemplateView.as_view(template_name='index.html'), name='home'),
    path('', user_views.home, name='home'),
]
