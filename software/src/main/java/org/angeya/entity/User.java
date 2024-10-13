package org.angeya.entity;

import lombok.Data;

import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;


/**
 * @Author: angeya
 * @Date: 2023/9/17 22:01
 * @Description:
 */
@Data
public class User {

    @NotNull
    private String name;

    @Min(value=0, message="年龄不允许为负数")
    private Integer age;


}
