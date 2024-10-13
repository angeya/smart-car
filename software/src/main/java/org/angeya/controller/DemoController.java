package org.angeya.controller;

import org.angeya.entity.User;
import org.angeya.vo.ResultVO;
import org.angeya.websocket.WebSocketHandler;
import org.springframework.validation.annotation.Validated;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.time.LocalDateTime;

/**
 * @Author: angeya
 * @Date: 2023/9/17 19:19
 * @Description:
 */
@RequestMapping("demo")
@RestController
public class DemoController {

    @Resource
    private WebSocketHandler webSocketHandler;

    @GetMapping("demo")
    public String demo() {
        return "OK";
    }

    @GetMapping("broadcastMsg")
    public String broadcastMsg() {
        LocalDateTime dateTime = LocalDateTime.now();
        String msg = "现在时间是：" + dateTime;
        webSocketHandler.broadcastMsg(msg);
        return "OK";
    }


    @GetMapping("redirect")
    public void redirect(HttpServletResponse response) throws IOException {
        response.sendRedirect("demo");
    }

    @PostMapping("getUser")
    public ResultVO getUser(@Validated @RequestBody User user) {
        return new ResultVO(user);
    }

}
