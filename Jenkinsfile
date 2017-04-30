#!groovy

// First experience: bullshit
/** 1. Workspace dir within Jenkins bin dir, not as configured.
    2. Escaping quotes in a bat-call is a pain, close to impossible.*/
pipeline {
    agent any

    stages {
        stage('Build') {
            steps {
                echo 'Building...'
                bat "cmake.exe -G 'Visual Studio 14 2015' -DBUILD_NR=${env.BUILD_NUMBER} -DQt5_version=5.8 -DQt_compiler_subdir=msvc2015 ${env.WORKSPACE}"
            }
        }
        stage('Deploy') {
            steps {
                echo 'Deploying...'
            }
        }
    }
}
