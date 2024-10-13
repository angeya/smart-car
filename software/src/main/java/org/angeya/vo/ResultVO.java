package org.angeya.vo;

import lombok.Data;
import org.angeya.result.ResultCode;
import org.angeya.result.StatusCode;

/**
 * @Author: angeya
 * @Date: 2023/9/17 21:57
 * @Description:
 */
@Data
public class ResultVO {

    // 状态码
    private int code;

    // 状态信息
    private String msg;

    // 返回对象
    private Object data;

    // 手动设置返回vo
    public ResultVO(int code, String msg, Object data) {


        this.code = code;
        this.msg = msg;
        this.data = data;
    }

    // 默认返回成功状态码，数据对象
    public ResultVO(Object data) {


        this.code = ResultCode.SUCCESS.getCode();
        this.msg = ResultCode.SUCCESS.getMsg();
        this.data = data;
    }

    // 返回指定状态码，数据对象
    public ResultVO(StatusCode statusCode, Object data) {


        this.code = statusCode.getCode();
        this.msg = statusCode.getMsg();
        this.data = data;
    }

    // 只返回状态码
    public ResultVO(StatusCode statusCode) {


        this.code = statusCode.getCode();
        this.msg = statusCode.getMsg();
        this.data = null;
    }
}